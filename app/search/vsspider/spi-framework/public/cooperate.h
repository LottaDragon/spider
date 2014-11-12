/**
 * $Id: cooperate.h,v 1.1 2009/04/16 12:26:55 pengdw Exp $
 */

#ifndef	_COOPERATE_H__
#define	_COOPERATE_H__

/* if there are prepared-id, thr wil finish & quit,
 * even M_EXIT is set by producer, else will quit even there are prepared-id.*/
#define M_STABLE		0100

/* id obtained by thr will follow an strict-order. */
#define M_STEPOVER		0200

/* try open ullib if set. */
#define M_ULLIB			0400

#ifndef	WITHOUT_ULLIB
#include "pub_Config.h"
#endif				/* ~ WITHOUT_ULLIB */

typedef int (co_callback) (int, void *);

class cooperate_t {
public:
    int Init(int nthr, int kids);

    /**
     * -1, errno,
     *  0, ok,
     *  +, val returned by co_producer,
     *     co_producer also may return -1, but better not do it;
     */

    int Run(co_callback * co_producer, void *var0, co_callback * co_consumer, void *var1,
	    int flags = 0);


    /* never-call this function, it's hack with pthread & c++. */
    int docallbacks(pthread_t myid);
    int GetMark() {
	return mark;
    }

    cooperate_t();
    ~cooperate_t();

private:
    int Run(int flags = 0);

    int nthr, kids;
    int initialized;

    co_callback *co_producer;
    void *var0;

    co_callback *co_consumer;
    void *var1;

    int mark, stepover;
    pthread_mutex_t mutex;
    pthread_cond_t empty_cond;
    pthread_cond_t prepared_cond;
    pthread_t *id;
    pthread_t *cache;
};

#endif				/* ~ _COOPERATE_H__ */
