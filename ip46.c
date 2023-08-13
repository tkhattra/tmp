#include <assert.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include "xutil.h"
#include "ip46.h"

int
ip46_from_sockaddr(IP46 *ip46, const struct sockaddr *sa, socklen_t sa_len)
{
    USED(sa_len);

    memset(ip46, 0, sizeof(*ip46));

    switch (sa->sa_family) {
    case AF_INET: {
        const struct sockaddr_in *sa4 = (const struct sockaddr_in *)sa;
        memcpy(ip46->ip, &sa4->sin_addr, 4);
        ip46->port = ntohs(sa4->sin_port);
        return 0;
    }

    case AF_INET6: {
        const struct sockaddr_in6 *sa6 = (const struct sockaddr_in6 *)sa;
        memcpy(ip46->ip, &sa6->sin6_addr, 16);
        ip46->port = ntohs(sa6->sin6_port);
        ip46->flag |= IP46_IS6;
        return 0;
    }

    default:
        errno = EPROTO;
        return -1;
    }
}

void
ip46_to_sockaddr(const IP46 *ip46, struct sockaddr *sa, socklen_t *sa_len)
{
    memset(sa, 0, *sa_len);

    if (ip46->flag & IP46_IS6) {
        struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *)sa;
        sa6->sin6_family = AF_INET6;
        memcpy(&sa6->sin6_addr, ip46->ip, 16);
        sa6->sin6_port = htons(ip46->port);
        *sa_len = sizeof(struct sockaddr_in6);
    } else {
        struct sockaddr_in *sa4 = (struct sockaddr_in *)sa;
        sa4->sin_family = AF_INET;
        memcpy(&sa4->sin_addr, ip46->ip, 4);
        sa4->sin_port = htons(ip46->port);
        *sa_len = sizeof(struct sockaddr_in);
    }
}

char *
ip46_ntop(const IP46 *ip46, char *str, size_t size)
{
    const char *ret;
    int af;

    af = ip46->flag & IP46_IS6 ? AF_INET6 : AF_INET;
    ret = inet_ntop(af, ip46->ip, str, size);
    assert(ret);
    return str;
}

// convert an IPv4-mapped IPv6 address to an IPv4 address
void
ip46_v4_mapped_to_v4(IP46 *ip46)
{
    // assert that ip46 is an IPv4-mapped IPv6 address
    assert(ip46->flag & IP46_IS6);
    assert(ip46_is_v4_mapped(ip46));

    ip46->ip[0] = ip46->ip[3];
    ip46->ip[1] = ip46->ip[2] = ip46->ip[3] = 0;
    ip46->flag &= ~IP46_IS6;
}

int
ip46_pton(IP46 *ip46, const char *str)
{
    if (inet_pton(AF_INET, str, ip46->ip) == 1) {
        ip46->ip[1] = ip46->ip[2] = ip46->ip[3] = 0;
        ip46->flag &= ~IP46_IS6;
        return 1;
    }

    if (inet_pton(AF_INET6, str, ip46->ip) == 1) {
        ip46->flag |= IP46_IS6;
        return 1;
    }

    return 0;
}

int
ip46_is_v4_mapped(const IP46 *ip46)
{
    return ip46->ip[0] == 0 && ip46->ip[1] == 0 && ip46->ip[2] == htonl(0xffff);
}

int
ip46_is_addr_unspecified(const IP46 *ip46)
{
    return ip46->ip[0] == 0 && ip46->ip[1] == 0 && ip46->ip[2] == 0 && ip46->ip[3] == 0;
}
