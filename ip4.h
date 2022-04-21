#ifndef IP4_H
#define IP4_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct IP4 IP4;

struct IP4
{
    char        ip[4];
    uint16_t    port;
};

#if defined(__cplusplus)
}
#endif

#endif
