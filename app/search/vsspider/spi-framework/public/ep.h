/**
 * $Id: ep.h,v 1.1 2009/02/23 14:29:30 pengdw Exp $
 */

#ifndef _EP_H_
#define _EP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <sys/epoll.h>

    int ep_init(int maxfds);
    void ep_free(void);

    int ep_del(int fd);
    int ep_add(int fd, unsigned events);
    int ep_mod(int fd, unsigned events);
    int ep_wait(struct epoll_event *events, int maxevents, int timeout);

    const char *strevents(unsigned events);
    void setnonblocking(int sock);

#ifdef __cplusplus
}
#endif
#endif				/* ~ _EP_H_ */
