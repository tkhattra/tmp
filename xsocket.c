#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include "xsocket.h"

// FIXME hack to work around <features.h> lunacy
#ifndef SOL_TCP
#define SOL_TCP 6
#endif

int xsocket_udp4(void)      { return socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); }
int xsocket_tcp4(void)      { return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); }
int xsocket_udp6(void)      { return socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP); }
int xsocket_tcp6(void)      { return socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP); }
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
xsocket_set_rcvtimeo(int sockfd, int usec)
{
    struct timeval tv;

    tv.tv_sec = usec/1000000;
    tv.tv_usec = usec%1000000;
    return setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

int
xsocket_set_sndtimeo(int sockfd, int usec)
{
    struct timeval tv;

    tv.tv_sec = usec/1000000;
    tv.tv_usec = usec%1000000;
    return setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
}

int
xsocket_tcpnodelay(int sockfd)
{
    int opt = 1;
    return setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
}

int
xsocket_bind(int sockfd, const IP46 *ip46)
{
    struct sockaddr_storage sa;
    socklen_t sa_len;

    if (ip46->flag & IP46_IS6) {
        struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *)&sa;
        memset(sa6, 0, sizeof(*sa6));
        sa6->sin6_family = AF_INET6;
        sa6->sin6_port = htons(ip46->port);
        memcpy(&sa6->sin6_addr, ip46->ip, 16);
        sa_len = sizeof(*sa6);
    } else {
        struct sockaddr_in *sa4 = (struct sockaddr_in *)&sa;
        memset(sa4, 0, sizeof(*sa4));
        sa4->sin_family = AF_INET;
        sa4->sin_port = htons(ip46->port);
        memcpy(&sa4->sin_addr, ip46->ip, 4);
        sa_len = sizeof(*sa4);
    }

    return bind(sockfd, (struct sockaddr *)&sa, sa_len);
}

int
xsocket_connect(int sockfd, const IP46 *ip46)
{
    struct sockaddr_storage sa;
    socklen_t sa_len;
    int ret;

    if (ip46->flag & IP46_IS6) {
        struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *)&sa;
        memset(sa6, 0, sizeof(*sa6));
        sa6->sin6_family = AF_INET6;
        sa6->sin6_port = htons(ip46->port);
        memcpy(&sa6->sin6_addr, ip46->ip, 16);
        sa_len = sizeof(*sa6);
    } else {
        struct sockaddr_in *sa4 = (struct sockaddr_in *)&sa;
        memset(sa4, 0, sizeof(*sa4));
        sa4->sin_family = AF_INET;
        sa4->sin_port = htons(ip46->port);
        memcpy(&sa4->sin_addr, ip46->ip, 4);
        sa_len = sizeof(*sa4);
    }

    do
        ret = connect(sockfd, (struct sockaddr *)&sa, sa_len);
    while ((ret < 0) && (errno == EINTR));

    return ret;
}

int
xsocket_accept(int sockfd, IP46 *ip46)
{
    struct sockaddr_storage sa;
    socklen_t sa_len = sizeof(sa);
    int ret;

    do
        ret = accept(sockfd, (struct sockaddr *)&sa, &sa_len);
    while ((ret < 0) && (errno == EINTR));

    if (ret >= 0) {
        if (ip46_from_sockaddr(ip46, (struct sockaddr *)&sa, sa_len) < 0)
            return -1;
    }

    return ret;
}

ssize_t
xsocket_recv(int sockfd, void *buf, size_t len, IP46 *ip46)
{
    struct sockaddr_storage sa;
    socklen_t sa_len = sizeof(sa);
    ssize_t ret;

    do
        ret = recvfrom(sockfd, buf, len, 0, (struct sockaddr *)&sa, &sa_len);
    while ((ret < 0) && (errno == EINTR));

    if (ret >= 0) {
        if (ip46_from_sockaddr(ip46, (struct sockaddr *)&sa, sa_len) < 0)
            return -1;
    }

    return ret;
}

ssize_t
xsocket_send(int sockfd, const void *buf, size_t len, const IP46 *ip46)
{
    struct sockaddr_storage sa;
    socklen_t sa_len;
    ssize_t ret;

    if (ip46->flag & IP46_IS6) {
        struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *)&sa;
        memset(sa6, 0, sizeof(*sa6));
        sa6->sin6_family = AF_INET6;
        sa6->sin6_port = htons(ip46->port);
        memcpy(&sa6->sin6_addr, ip46->ip, 16);
        sa_len = sizeof(*sa6);
    } else {
        struct sockaddr_in *sa4 = (struct sockaddr_in *)&sa;
        memset(sa4, 0, sizeof(*sa4));
        sa4->sin_family = AF_INET;
        sa4->sin_port = htons(ip46->port);
        memcpy(&sa4->sin_addr, ip46->ip, 4);
        sa_len = sizeof(*sa4);
    }

    do
        ret = sendto(sockfd, buf, len, 0, (struct sockaddr *)&sa, sa_len);
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
