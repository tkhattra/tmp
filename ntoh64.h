#ifndef NTOH64_H
#define NTOH64_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

extern uint64_t ntoh64(uint64_t ull);
extern uint64_t hton64(uint64_t ull);

#if defined(__cplusplus)
}
#endif

#endif
