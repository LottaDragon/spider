/**
 * $Id: sendpack.h,v 1.1 2009/04/27 09:53:28 pengdw Exp $
 */

#ifndef	_sendpack_h__
#define	_sendpack_h__

#include "mcpack.h"
#include "serv.h"

class abserv_t {
public:
    char name [32];
    char hostname [128];
    int port;

    int connect_timeout;
    int read_timeout;
    int write_timeout;

    int initretry_delay;
    int maxretry_delay;

    int keepalive;
    int retryutil;
    int noackpack;
    int ismcpackvalid;

    /* only for tool <mcpack>. */
    int continuous_read;
    int output_text;

    abserv_t();
    int sendpack(mc_pack_t *inpack, char *tmpbuf, int max_tmpbuf_size);

    /* only by allserv, a map -> serv_*() */
    int Init(const char *name, balancer_t balancer);
    int Add(const char *ipaddr, int port);
    serv1_t *Get(const char *req = NULL);
    int Disable(serv1_t *serv1, int seconds);

private:
    int sendpack1(mc_pack_t *inpack, char *tmpbuf, int max_tmpbuf_size);

    serv_t allserv;
};

#endif	/* ~ _sendpack_h__ */
