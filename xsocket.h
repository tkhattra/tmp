#ifndef XSOCKET_H
#define XSOCKET_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "ip46.h"

#ifndef UNIX_PATH_MAX
#define UNIX_PATH_MAX 108
#endif

extern int      xsocket_udp4(void);
extern int      xsocket_tcp4(void);
extern int      xsocket_udp6(void);
extern int      xsocket_tcp6(void);
extern int      xsocket_unixdgram(void);

extern int      xsocket_bind(int sockfd, const IP46 *ip46);
extern int      xsocket_accept(int sockfd, IP46 *ip46);
extern int      xsocket_connect(int sockfd, const IP46 *ip46);
extern ssize_t  xsocket_recv(int sockfd, void *buf, size_t len, IP46 *ip46);
extern ssize_t  xsocket_send(int sockfd, const void *buf, size_t len, const IP46 *ip46);

int             xsocket_bindunix(int sockfd, const char *path);
ssize_t         xsocket_recvunix(int sockfd, void *buf, size_t len, char *path, size_t pathlen);
ssize_t         xsocket_sendunix(int sockfd, const void *buf, size_t len, const char *path);

extern int      xsocket_listen(int sockfd, int backlog);
extern int      xsocket_reuseaddr(int sockfd);
extern int      xsocket_tcpnodelay(int sockfd);
extern int      xsocket_keepalive(int sockfd);
extern int      xsocket_keepidle(int sockfd, int keepidle);
extern int      xsocket_set_rcvtimeo(int sockfd, int usec);
extern int      xsocket_set_sndtimeo(int sockfd, int usec);

extern void     xsocket_set_rcvbuf(int sockfd, int size);
extern void     xsocket_set_sndbuf(int sockfd, int size);
extern int      xsocket_get_rcvbuf(int sockfd);
extern int      xsocket_get_sndbuf(int sockfd);

extern int      xsocket_tmperror(int errnum);
extern int      xsocket_tcpeof(ssize_t nread, int errnum);
extern int      xsocket_noconn(int errnum);

#if defined(__cplusplus)
}
#endif

#endif
