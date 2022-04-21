#include <errno.h>
#include "xpoll.h"

int
xpoll(struct pollfd *fds, nfds_t nfds, int timeout_msec)
{
    nfds_t i;
    int ret;

    for (i=0; i<nfds; i++)
        fds[i].revents = 0;
    do
        ret = poll(fds, nfds, timeout_msec);
    while ((ret < 0) && (errno == EINTR));
    return ret;
}

int xpollfd_error(struct pollfd *fd)  { return fd->revents & POLLNVAL; }
int xpollfd_except(struct pollfd *fd) { return fd->revents & (POLLERR|POLLNVAL); }
int xpollfd_read(struct pollfd *fd)   { return fd->revents & (POLLIN|POLLHUP); }
int xpollfd_write(struct pollfd *fd)  { return fd->revents & POLLOUT; }
