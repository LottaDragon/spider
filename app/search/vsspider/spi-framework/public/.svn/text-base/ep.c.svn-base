/**
 * $Id: ep.c,v 1.1 2009/02/23 14:29:30 pengdw Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <signal.h>

#include <fcntl.h>
#include <sys/epoll.h>

#include "ep.h"

static int epfd;

#define dumpcorefile(fmt, args...) \
do { \
        fprintf (stderr, "--- FATAL --- %s.%d: (dump) "fmt, __FILE__, __LINE__, ##args); \
	raise (SIGABRT); \
} while (0)

void ep_free(void)
{
    if (epfd)
	close(epfd);
}

int ep_del(int fd)
{
    struct epoll_event ev;

    assert(epfd);
    memset(&ev, 0, sizeof(ev));

    if (0 != epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev)) {
	dumpcorefile("epoll_ctl (del) failed on fd %d, %m\n", fd);
    }

    return 0;
}

int ep_add(int fd, unsigned events)
{
    struct epoll_event ev;

    assert(epfd);
    memset(&ev, 0, sizeof(ev));

    if (events & EPOLLIN)
	ev.events |= EPOLLIN;
    if (events & EPOLLOUT)
	ev.events |= EPOLLOUT;

    ev.events |= (EPOLLERR | EPOLLHUP);
    ev.data.fd = fd;

    if (0 != epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev)) {
	dumpcorefile("epoll_ctl (add) failed on fd %d, %m\n", fd);
    }

    return 0;
}

int ep_mod(int fd, unsigned events)
{
    struct epoll_event ev;

    assert(epfd);
    memset(&ev, 0, sizeof(ev));

    if (events & EPOLLIN)
	ev.events |= EPOLLIN;
    if (events & EPOLLOUT)
	ev.events |= EPOLLOUT;

    ev.events |= (EPOLLERR | EPOLLHUP);
    ev.data.fd = fd;

    if (0 != epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev)) {
	dumpcorefile("epoll_ctl (mod) failed on fd %d, %m\n", fd);
    }

    return 0;
}

int ep_init(int maxfds)
{
    if (-1 == (epfd = epoll_create(maxfds))) {
	dumpcorefile("epoll_create failed, maxfds %d, %m\n", maxfds);
	return -1;
    }

    if (-1 == fcntl(epfd, F_SETFD, FD_CLOEXEC)) {
	close(epfd);
	dumpcorefile("epoll_create, fcntl FD_CLOEXEC failed, epfd %d, %m\n", epfd);

	return -1;
    }

    return 0;
}

int ep_wait(struct epoll_event *events, int maxevents, int timeout)
{
    return epoll_wait(epfd, events, maxevents, timeout);
}

/**
 * misc functions
 */

const char *strevents(unsigned events)
{
    size_t flag = 0, offset = 0;
    static char line[LINE_MAX];

    line[0] = 0;

#define __strevents_cat(var) \
do { \
	if (events & var) { \
		offset += snprintf (line + offset, sizeof (line) - offset, \
			(flag ++?", %s":"%s"), #var); \
		if (offset >= sizeof (line)) return line; \
	} \
} while (0)

    __strevents_cat(EPOLLIN);
    __strevents_cat(EPOLLOUT);
    __strevents_cat(EPOLLHUP);
    __strevents_cat(EPOLLERR);

    return line;
}

void setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock, F_GETFL);
    if (opts < 0) {
	dumpcorefile("fcntl (sock,GETFL) ");
	exit(1);
    }

    opts = opts | O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0) {
	dumpcorefile("fcntl (sock,SETFL,opts) ");
	exit(1);
    }

    if (fcntl(sock, F_SETFD, FD_CLOEXEC) < 0) {
	dumpcorefile("fcntl (sock,F_SETFD,FD_CLOEXEC) ");
	exit(1);
    }
}
