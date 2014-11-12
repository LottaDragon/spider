#include <stdio.h>
#include <stdlib.h>

#include "ul_net.h"
#include "nshead.h"
#include "mcpack.h"
#include "nsAutoPtr.h"
#include "misc.h"

#include "sendpack.h"

abserv_t::abserv_t() {
    bzero (name, sizeof(name));
    bzero (hostname, sizeof (hostname));
    port = 0;

    connect_timeout = 500;
    read_timeout = 3000;
    write_timeout = 2000;

    initretry_delay = 500;
    maxretry_delay = 5000;

    keepalive = 0;
    retryutil = 0;
    noackpack = 0;
    ismcpackvalid = 0;

    continuous_read = 0;
    output_text = 0;
}

int abserv_t::sendpack1(mc_pack_t *inpack, char *tmpbuf, int max_tmpbuf_size) {
    int sock,
	e = 0;

    serv1_t uniqif;
    serv1_t *serv1;

    if (*hostname) {
	strlcpy (uniqif.ipaddr, hostname, sizeof(uniqif.ipaddr));
	uniqif.port = port;
	serv1 = &uniqif;
    }
    else {
	serv1 = Get();
	if (!serv1) {
	    WARNING("no-active abserv found, maybe all dead!!!");
	    return -1;
	}
    }

    if ((sock = ul_tcpconnecto_ms(serv1->ipaddr, serv1->port, connect_timeout)) == -1) {
        WARNING("connect to %s{\"%s:%d\"} failed, %m", name, serv1->ipaddr, serv1->port);
        return -1;
    }

    nsAutoPtr ap(sock);

    nshead_t *nshead = (nshead_t *)tmpbuf;
    nshead->body_len = mc_pack_get_size(inpack);
    int bodylen = nshead->body_len;

    if ((e = nshead_write(sock, nshead, mc_pack_get_buffer(inpack), write_timeout)) < 0) {
        WARNING("send pack %s{\"%s:%d\"} failed, %s, %m", name, serv1->ipaddr, serv1->port, nshead_error(e));
        return -1;
    }

    if (noackpack)
        return 0;

    do {
        if ((e = nshead_read(sock, tmpbuf, max_tmpbuf_size, read_timeout)) < 0) {
            WARNING("read pack %s{\"%s:%d\"} failed, %s, %m", name, serv1->ipaddr, serv1->port, nshead_error(e));
            return -1;
        }

        if (!ismcpackvalid)
            return 0;

        if (nshead->body_len == 0) {
            WARNING("recv pack, mcpack invalid.");
            return -1;
        }

        PREPARE_MCPACK_readonly(tmpbuf, max_tmpbuf_size, _inp);
        if (MC_PACK_PTR_ERR(mcpack_inp)) {
            WARNING("recv pack, mcpack invalid.");
            return -1;
        }

        if (output_text) {
            char *mcpacktext = (char *) malloc (102400);
            if (mcpacktext) {
                if (mc_pack_pack2text(mcpack_inp, mcpacktext, 102400, 0) != 0) {
                    WARNING("mcpack pack2text failed!");
                } else
                    fprintf(stdout, " --> \"%s\"\n", mcpacktext);
            }

            free (mcpacktext);
        }

    } while (continuous_read);

    TRACE("send a mcpack{%d} to %s{\"%s:%d\"} ok, {resp-size %d}.",
          bodylen, name, hostname, port, nshead->body_len);
    return 0;
}

int abserv_t::sendpack(mc_pack_t *inpack, char *tmpbuf, int max_tmpbuf_size) {
    int retryutil0 = retryutil;

    int retrydelay = initretry_delay;
    if (retrydelay < 0)
        retrydelay = 0;

    int e = 0;

    do {
        if ((e = sendpack1(inpack, tmpbuf, max_tmpbuf_size)) == -1 && retryutil0 != 0) {
            if (retrydelay < maxretry_delay) {

                if (retrydelay)
                    Nanosleep(retrydelay);

                if (retrydelay <= 0)
                    retrydelay = 10;

                retrydelay <<= 1;
            } else {
                if (maxretry_delay > 0)
                    Nanosleep(maxretry_delay);
            }
        } else break;
    } while (retryutil0 < 0 || retryutil0 -- > 0);

    return e;
}

int abserv_t::Init(const char *name, balancer_t balancer)
{
    hostname[0] = '\0';
    return serv_initial(&allserv, name, balancer);
}

int abserv_t::Add(const char *ipaddr, int port)
{
    return serv_add(&allserv, ipaddr, port);
}

serv1_t *abserv_t::Get(const char *req)
{
    return serv_get(&allserv, req);
}

int abserv_t::Disable(serv1_t *serv1, int seconds)
{
    return serv_disable(serv1, seconds);
}
