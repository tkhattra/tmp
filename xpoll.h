#ifndef XPOLL_H
#define XPOLL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <poll.h>

extern int  xpoll(struct pollfd *fds, nfds_t nfds, int timeout_msec);
extern int  xpollfd_error(struct pollfd *fd);
extern int  xpollfd_except(struct pollfd *fd);
extern int  xpollfd_read(struct pollfd *fd);
extern int  xpollfd_write(struct pollfd *fd);

#if defined(__cplusplus)
}
#endif

#endif
