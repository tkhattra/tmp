#ifndef IP46_H
#define IP46_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <sys/socket.h>
#include <netinet/in.h>

typedef struct IP46 IP46;

struct IP46
{
    uint32_t    ip[4];  // network byte order
    uint16_t    port;   // host byte order
    uint16_t    flag;   // bitmask of values in enum below
};

// IP46.flag bitmask values
enum
{
    IP46_IS6 = (1<<0),
};

extern int  ip46_from_sockaddr(IP46 *ip46, const struct sockaddr *sa, socklen_t sa_len);
extern char *ip46_ntop(const IP46 *ip46, char *str, size_t size);
extern void ip46_v4_mapped_to_v4(IP46 *ip46);
extern int  ip46_pton(IP46 *ip46, const char *str);

#if defined(__cplusplus)
}
#endif

#endif
