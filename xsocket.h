#ifndef XSOCKET_H
#define XSOCKET_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "ip4.h"

#ifndef UNIX_PATH_MAX
#define UNIX_PATH_MAX 108
#endif

extern int      xsocket_udp(void);
extern int      xsocket_tcp(void);
extern int      xsocket_unixdgram(void);

extern int      xsocket_bind4(int sockfd, IP4 ip4);
extern int      xsocket_accept4(int sockfd, IP4 *ip4);
extern int      xsocket_connect4(int sockfd, IP4 ip4);
extern ssize_t  xsocket_recv4(int sockfd, void *buf, size_t len, IP4 *ip4);
extern ssize_t  xsocket_send4(int sockfd, const void *buf, size_t len, IP4 ip4);

int             xsocket_bindunix(int sockfd, const char *path);
ssize_t         xsocket_recvunix(int sockfd, void *buf, size_t len, char *path, size_t pathlen);
ssize_t         xsocket_sendunix(int sockfd, const void *buf, size_t len, const char *path);

extern int      xsocket_listen(int sockfd, int backlog);
extern int      xsocket_reuseaddr(int sockfd);
extern int      xsocket_tcpnodelay(int sockfd);
extern int      xsocket_keepalive(int sockfd);
extern int      xsocket_keepidle(int sockfd, int keepidle);

extern void     xsocket_set_rcvbuf(int sockfd, int size);
extern void     xsocket_set_sndbuf(int sockfd, int size);
extern int      xsocket_get_rcvbuf(int sockfd);
extern int      xsocket_get_sndbuf(int sockfd);

extern int      xsocket_tmperror(int errnum);
extern int      xsocket_tcpeof(ssize_t nread, int errnum);
extern int      xsocket_noconn(int errnum);

extern int      xinet4_pton(const void *src, char *dst);
extern int      xinet6_pton(const void *src, char *dst);

#if defined(__cplusplus)
}
#endif

#endif
