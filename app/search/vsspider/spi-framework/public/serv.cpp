#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

#include "serv.h"

#define strlcpy(dst, src, size) snprintf ((dst), (size), "%s", (src))

int serv_initial(serv_t * serv, const char *name, balancer_t balancer)
{
    assert(serv);
    memset(serv, 0, sizeof(*serv));

    if (name)
	strlcpy(serv->name, name, sizeof(serv->name));
    else
	serv->name[0] = '\0';

    serv->balancer = balancer;

    // serv->size = 0;
    serv->capability = 32;

    serv->allserv1 = (serv1_t *) calloc(serv->capability, sizeof(serv1_t));
    if (!serv->allserv1)
	return -1;

    srand(time(0) ^ getpid());
    return 0;
}

int serv_add(serv_t * serv, const char *ipaddr, int port)
{
    assert(serv);

    if (serv->size >= serv->capability) {
	size_t newcapability = serv->capability;

	while (newcapability <= serv->size)
	    newcapability *= 2;

	void *memory = realloc(serv->allserv1, newcapability * sizeof(serv1_t));
	if (memory == NULL) {
	    // realloc failed, leave serv UN-changed.
	    return -1;
	}

	serv->capability = newcapability;
	serv->allserv1 = (serv1_t *) memory;
    }

    assert(serv->size < serv->capability);

    memset(&(serv->allserv1[serv->size]), 0, sizeof(serv1_t));
    strlcpy(serv->allserv1[serv->size].ipaddr, ipaddr, sizeof(serv->allserv1[serv->size].ipaddr));
    serv->allserv1[serv->size].ipaddr_size = strlen(serv->allserv1[serv->size].ipaddr);
    serv->allserv1[serv->size].port = port;
    serv->allserv1[serv->size].idx = serv->size;
    // serv->allserv1[serv->size].disabled = 0;

    serv->size++;
    return 0;
}

#include <openssl/md5.h>

serv1_t *serv_get(serv_t * serv, const char *req)
{
    assert(serv);

    serv1_t *serv1 = NULL;
    time_t now = time(0);
    int i;

    switch (serv->balancer) {
    case S_STATIC:
	for (i = 0; i < (int) serv->size; i++) {
	    if (serv->allserv1[i].disabled <= now) {
		serv1 = &(serv->allserv1[i]);
		break;
	    }
	}
	break;
    case S_MD5SUM_CARP:
	int len;
	if (req && (len = strlen(req))) {
	    MD5_CTX md5ctx1, md5ctx;
	    MD5_Init(&md5ctx1);
	    MD5_Update(&md5ctx1, req, len);

	    uint64_t md5sum[3], checksum, maxchecksum = 0;

	    for (i = 0; i < (int) serv->size; i++) {
		if (serv->allserv1[i].disabled <= now) {
		    md5ctx = md5ctx1;
		    MD5_Update(&md5ctx, serv->allserv1[i].ipaddr, serv->allserv1[i].ipaddr_size);
		    MD5_Update(&md5ctx, &(serv->allserv1[i].port), sizeof(serv->allserv1[i].port));
		    MD5_Final((unsigned char *) md5sum, &md5ctx);

		    checksum = md5sum[0] + md5sum[1];

		    if (checksum > maxchecksum) {
			serv1 = &(serv->allserv1[i]);
			maxchecksum = checksum;
		    }
		}
	    }

	    break;
	}
	// drop down to Round-Robin when req is empty ...
    case S_RR:
    default:
	int sumx = 0;
	for (i = 0; i < (int) serv->size; i++) {
	    if (serv->allserv1[i].disabled <= now) {
		sumx++;
		if (rand() % sumx == 0)
		    serv1 = &(serv->allserv1[i]);
	    }
	}
	break;
    }

    return serv1;
}

int serv_disable (serv1_t *serv1, int seconds)
{
	if (serv1)
	    serv1->disabled = time (0) + seconds;

	return 0;
}

#ifdef	_SERV_CREATE_TEST_SUITE__

int main(int argc, char **argv)
{
    int i, loop = 1000, ty = 0;
    if (argc > 1)
	loop = atoi(argv[1]);

    if (argc > 2)
	ty = atoi(argv[2]);

    serv_t serv;

    if (serv_initial(&serv, "sunflower", (balancer_t) ty) == -1) {
	fprintf(stderr, "serv_initial failed.");
	return -1;
    }
#define MAXSERV 16

    int n = MAXSERV;
    int count[MAXSERV];

    memset(count, 0, sizeof(count));

    char tmpbuf[MAXSERV];
    for (i = 0; i < n; i++) {
	snprintf(tmpbuf, sizeof(tmpbuf), "127.0.%d.%d", (i >> 8) % 256, i % 256);

	int r = serv_add(&serv, tmpbuf, 0);
	assert(r == 0);
    }

    for (i = 0; i < loop; i++) {
	int randreq = rand();
	// randreq = 102;

	snprintf(tmpbuf, sizeof(tmpbuf), "%d", randreq);

	serv1_t *serv1 = serv_get(&serv, tmpbuf);
	assert(serv1);

	count[serv1->idx]++;
    }

    for (i = 0; i < MAXSERV; i++) {
	printf("%5d", count[i]);
	if (i % 32 == 31)
	    printf("\n");
    }

    return 0;
}

#endif				/* ~ _SERV_CREATE_TEST_SUITE__ */
