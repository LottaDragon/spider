/**
 * $Id: serv.h,v 1.1 2009/04/27 09:53:28 pengdw Exp $
 */

#ifndef	_SERV_H__
#define	_SERV_H__

typedef struct {
    char ipaddr[32];
    size_t ipaddr_size;
    int port;
    time_t disabled;
    int idx;
} serv1_t;

typedef enum {
    S_RR,
    S_MD5SUM_CARP,
    S_STATIC,
} balancer_t;

typedef struct {
    char name[32];
    balancer_t balancer;

    size_t size, capability;
    serv1_t *allserv1;
} serv_t;

int serv_initial(serv_t * serv, const char *name, balancer_t balancer);
int serv_add(serv_t * serv, const char *ipaddr, int port);
serv1_t *serv_get(serv_t * serv, const char *req = NULL);
int serv_disable (serv1_t *serv1, int seconds);

#endif				/* ~ _SERV_H__ */
