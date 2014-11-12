#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

#include <pthread.h>

#include "cooperate.h"
#include "misc.h"

#define M_MUTEX			01
#define M_EMPTY_COND		02
#define M_PREPARED_COND		04

#define M_EXIT			040

#ifndef	_COOPERATE_DEBUG
#ifdef	TRACE
#undef	TRACE
#endif	/* ~ TRACE */
#define TRACE(fmt, args...) (void)(fmt)
#endif				/* ~ _COOPERATE_DEBUG */

#define MAXID ((pthread_t) -1)

static
void *onethr(void *cop0)
{
    pthread_t myid;
    cooperate_t *cop = (cooperate_t *)cop0;

    myid = pthread_self();

    // pthread_t will never be MAXID & 0,
    // it's really a ptr-addr by malloc.
    assert(myid != MAXID);
    assert(myid != 0);

#ifndef	WITHOUT_ULLIB
    ulNewThread New;
    if (cop->GetMark() & M_ULLIB)
	New.Run("cooperate", 0);
#endif				/* ~ WITHOUT_ULLIB */

    cop->docallbacks(myid);
    return (void*)0;
}

int cooperate_t::Init(int nthr_, int kids_)
{
    int flags = 0;

    if (initialized)
	return 0;

    nthr = nthr_;
    kids = kids_;

    do {
	if ((id = (pthread_t *) calloc(nthr, sizeof(pthread_t))) == NULL)
	    return -1;
	if ((cache = (pthread_t *) calloc(kids, sizeof(cache[0]))) == NULL)
	    break;

	if (pthread_mutex_init(&mutex, 0) != 0) break;
	else flags |= M_MUTEX;
	if (pthread_cond_init(&empty_cond, 0) != 0) break;
	else flags |= M_EMPTY_COND;
	if (pthread_cond_init(&prepared_cond, 0) != 0) break;
	else flags |= M_PREPARED_COND;
    } while (0);

    if (flags != (M_MUTEX | M_EMPTY_COND | M_PREPARED_COND)) {
	if (flags & M_MUTEX) pthread_mutex_destroy(&mutex);
	if (flags & M_EMPTY_COND) pthread_cond_destroy(&empty_cond);
	if (flags & M_PREPARED_COND) pthread_cond_destroy(&prepared_cond);
	if (id) free(id);
	if (cache) free(cache);
	initialized = 0;
	return -1;
    }

    initialized = 1;
    return 0;
}

int cooperate_t::docallbacks(pthread_t myid)
{
    int i, idx;

    while (1) {
	idx = -1;

	pthread_mutex_lock(&mutex);

	do {
	    int k;

	    for (i = 0; i < kids; i++) {
		k = ((mark & M_STEPOVER) ? ((i + stepover) % kids) : i);
		if (cache[k] == MAXID)
		    break;
	    }

	    if (i < kids) {
		if ((mark & M_EXIT) && (mark & M_STABLE) == 0)
		    break;

		idx = k;
		cache[k] = myid;

		if (mark & M_STEPOVER)
		    stepover = k + 1;
	    } else {
		if ((mark & M_EXIT))
		    break;

		TRACE(" x, %d, wait prepared_cond\nthrs", (int) myid);
		pthread_cond_wait(&prepared_cond, &mutex);
	    }
	} while (idx == -1);

	pthread_mutex_unlock(&mutex);

	if (idx == -1)
	    break;

	// we got idx ...
	if ((*co_consumer) (idx, var1))
	    break;

	// idx finished ...
	pthread_mutex_lock(&mutex);
	TRACE(" <<<, idx %d, %d\nthrs", idx, (int) myid);
	cache[idx] = 0;
	pthread_cond_broadcast(&empty_cond);
	pthread_mutex_unlock(&mutex);
    }

    return 0;
}

int cooperate_t::Run(int flags)
{
    int i, r = 0;

    if (!initialized || nthr < 0 || kids < 0
	    || !co_producer || !co_consumer) {
	errno = EINVAL;
	return -1;
    }

    mark = (flags & (M_STABLE | M_STEPOVER | M_ULLIB));

    do {
	for (i = 0; i < nthr; i++) {
	    r = pthread_create(&id[i], 0, onethr, (void *) this);
	    if (r != 0) {
		// we should cancel all started consumer,
		// because producer hasn't started yet, they are
		// blocked on prepared_cond or starting.
		// so we pthread_cancel everyone started and then pthread_join.
		id[i] = 0;
		break;
	    }
	}

	if (r) break;

	while (1) {
	    int idx = -1;

	    pthread_mutex_lock(&mutex);
	    do {
		for (i = 0; i < kids; i++) {
		    if (cache[i] == 0)
			break;
		}

		if (i < kids)
		    idx = i;
		else {
		    TRACE(" x, wait empty_cond\nthrs");
		    pthread_cond_wait(&empty_cond, &mutex);
		}
	    } while (idx == -1);
	    pthread_mutex_unlock(&mutex);

	    // we got idx ...
	    r = (*co_producer) (idx, var0);

	    pthread_mutex_lock(&mutex);

	    if (r) {
		// producer wants quit.
		mark |= M_EXIT;

		// broadcast to unblock blocked one,
		// and tell them to exit.
		pthread_cond_broadcast(&prepared_cond);
	    } else {
		TRACE(" >>>, idx %d\nthrs", idx);
		cache[idx] = MAXID;
		pthread_cond_broadcast(&prepared_cond);
	    }

	    pthread_mutex_unlock(&mutex);

	    if (r) break;
	}
    } while (0);

    int saved_errno = 0;
    if (r && (mark & M_EXIT) == 0)
	saved_errno = errno;

    // when normal M_EXIT, we don't do pthread_cancel,
    // only wait for consumer finished.
    if ((mark & M_EXIT) == 0) {
	// we must lock, because thr maybe in crit-block before cond_wait.
	// then the cancel will no work.
	pthread_mutex_lock(&mutex);
	for (i = 0; i < nthr; i++) {
	    if (id[i])
		pthread_cancel(id[i]);
	}
	pthread_mutex_unlock(&mutex);
    }

    for (i = 0; i < nthr; i++) {
	if (id[i]) {
	    pthread_join(id[i], 0);
	    id[i] = (pthread_t) 0;
	}
    }

    errno = saved_errno;
    return r;
}

cooperate_t::cooperate_t()
{
    nthr = kids = initialized = 0;

    co_producer = co_consumer = NULL;
    var0 = var1 = 0;

    mark = stepover = 0;
    id = cache = NULL;
}

cooperate_t::~cooperate_t()
{
    if (initialized) {
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&empty_cond);
	pthread_cond_destroy(&prepared_cond);
	free(id);
	free(cache);

	initialized = 0;
    }
}

int cooperate_t::Run(co_callback *co_producer_, void *var0_,
	co_callback *co_consumer_, void *var1_,
	int flags)
{
#define __co_pad0(name) name = name##_;
    __co_pad0(co_producer);
    __co_pad0(var0);
    __co_pad0(co_consumer);
    __co_pad0(var1);
#undef __co_pad0

    return Run(flags);
}
