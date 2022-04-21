#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include "xsocket.h"

// FIXME hack to work around <features.h> lunacy
#ifndef SOL_TCP
#define SOL_TCP 6
#endif

int xsocket_udp(void)       { return socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); }
int xsocket_tcp(void)       { return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); }
int xsocket_unixdgram(void) { return socket(AF_UNIX, SOCK_DGRAM, 0); }

int xsocket_listen(int sockfd, int backlog) { return listen(sockfd, backlog); }

int
xsocket_reuseaddr(int sockfd)
{
    int opt = 1;
    return setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

int
xsocket_keepalive(int sockfd)
{
    int opt = 1;
    return setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
}

int
xsocket_keepidle(int sockfd, int keepidle)
{
    int ret = 0;
#ifdef TCP_KEEPIDLE
    ret = setsockopt(sockfd, SOL_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle));
#endif
    return ret;
}

int
xsocket_tcpnodelay(int sockfd)
{
    int opt = 1;
    return setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
}

int
xsocket_bind4(int sockfd, IP4 ip4)
{
    struct sockaddr_in sa;

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(ip4.port);
    memcpy(&sa.sin_addr, ip4.ip, 4);

    return bind(sockfd, (struct sockaddr *)&sa, sizeof(sa));
}

extern int
xsocket_connect4(int sockfd, IP4 ip4)
{
    struct sockaddr_in sa;
    int ret;

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(ip4.port);
    memcpy(&sa.sin_addr, ip4.ip, 4);

    do
        ret = connect(sockfd, (struct sockaddr *)&sa, sizeof(sa));
    while ((ret < 0) && (errno == EINTR));

    return ret;
}

int
xsocket_accept4(int sockfd, IP4 *ip4)
{
    struct sockaddr_in sa;
    int ret;
    socklen_t dummy = sizeof(sa);

    do
        ret = accept(sockfd, (struct sockaddr *)&sa, &dummy);
    while ((ret < 0) && (errno == EINTR));

    if (ret >= 0) {
        memcpy(ip4->ip, &sa.sin_addr, 4);
        ip4->port = ntohs(sa.sin_port);
    }

    return ret;
}

ssize_t
xsocket_recv4(int sockfd, void *buf, size_t len, IP4 *ip4)
{
    struct sockaddr_in sa;
    socklen_t addrlen = sizeof(sa);
    ssize_t ret;

    do
        ret = recvfrom(sockfd, buf, len, 0, (struct sockaddr *)&sa, &addrlen);
    while ((ret < 0) && (errno == EINTR));

    if (ret >= 0) {
        memcpy(ip4->ip, &sa.sin_addr, 4);
        ip4->port = ntohs(sa.sin_port);
    }

    return ret;
}

ssize_t
xsocket_send4(int sockfd, const void *buf, size_t len, IP4 ip4)
{
    struct sockaddr_in sa;
    ssize_t ret;

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(ip4.port);
    memcpy(&sa.sin_addr, ip4.ip, 4);

    do
        ret = sendto(sockfd, buf, len, 0, (struct sockaddr *)&sa, sizeof(sa));
    while ((ret < 0) && (errno == EINTR));

    return ret;
}

int
xsocket_bindunix(int sockfd, const char *path)
{
    struct sockaddr_un sa;

    memset(&sa, 0, sizeof(sa));
    sa.sun_family = AF_UNIX;
    snprintf(sa.sun_path, sizeof(sa.sun_path), "%s", path);

    return bind(sockfd, (struct sockaddr *)&sa, sizeof(sa));
}

ssize_t
xsocket_recvunix(int sockfd, void *buf, size_t len, char *path, size_t pathlen)
{
    struct sockaddr_un sa;
    socklen_t addrlen = sizeof(sa);
    ssize_t ret;

    do
        ret = recvfrom(sockfd, buf, len, 0, (struct sockaddr *)&sa, &addrlen);
    while ((ret < 0) && (errno == EINTR));

    if (ret >= 0)
        snprintf(path, pathlen, "%s", sa.sun_path);

    return ret;
}

ssize_t
xsocket_sendunix(int sockfd, const void *buf, size_t len, const char *path)
{
    struct sockaddr_un sa;
    int ret;

    memset(&sa, 0, sizeof(sa));
    sa.sun_family = AF_UNIX;
    snprintf(sa.sun_path, sizeof(sa.sun_path), "%s", path);

    do
        ret = sendto(sockfd, buf, len, 0, (struct sockaddr *)&sa, sizeof(sa));
    while ((ret < 0) && (errno == EINTR));

    return ret;
}

static void
xsocket_set_xbuf(int sockfd, int size, int optname)
{
    while (size >= 1024) {
        if (setsockopt(sockfd, SOL_SOCKET, optname, &size, sizeof(size)) == 0)
            return;
        size -= size>>5;
    }
}

void xsocket_set_rcvbuf(int sockfd, int size) { xsocket_set_xbuf(sockfd, size, SO_RCVBUF); }
void xsocket_set_sndbuf(int sockfd, int size) { xsocket_set_xbuf(sockfd, size, SO_SNDBUF); }

static int
xsocket_get_xbuf(int sockfd, int optname)
{
    int size;
    socklen_t len = sizeof(size);

    if (getsockopt(sockfd, SOL_SOCKET, optname, &size, &len))
        return -1;
    return size;
}

int xsocket_get_rcvbuf(int sockfd) { return xsocket_get_xbuf(sockfd, SO_RCVBUF); }
int xsocket_get_sndbuf(int sockfd) { return xsocket_get_xbuf(sockfd, SO_SNDBUF); }

int
xsocket_tmperror(int errnum)
{
    switch (errnum) {
    case EAGAIN:
    case ENOBUFS:
    case ENOMEM:
    case EHOSTUNREACH:
    case ENETUNREACH:
    case EHOSTDOWN:
    case ENETDOWN:
        return 1;
    default:
        return 0;
    }
}

// return true if tcp EOF after call to read(2)
int
xsocket_tcpeof(ssize_t nread, int errnum)
{
    if (nread > 0)
        return 0;

    if (nread == 0)
        return 1;

    // nread < 0
    switch (errnum) {
    case ECONNRESET:
    case EPIPE:
    case ETIMEDOUT:
    case EHOSTUNREACH:
    case ENETUNREACH:
    case EHOSTDOWN:
    case ENETDOWN:
        return 1;
    default:
        return 0;
    }
}

// return true if no connection after call to accept(2)
int
xsocket_noconn(int errnum)
{
    switch (errnum) {
    case EAGAIN:
    case ECONNABORTED:
        return 1;
    default:
        return 0;
    }
}

int
xinet_pton(int af, const void *src, char *dst)
{
    return inet_pton(af, src, dst);
}

int  xinet4_pton(const void *src, char *dst) { return xinet_pton(AF_INET, src, dst); }
int  xinet6_pton(const void *src, char *dst) { return xinet_pton(AF_INET6, src, dst); }
