#include "ntoh64.h"

uint64_t
ntoh64(uint64_t ull)
{
    uint64_t ret;
    uint8_t *ptr = (uint8_t *)&ret;

    ptr[0] = ull >> 56;
    ptr[1] = ull >> 48;
    ptr[2] = ull >> 40;
    ptr[3] = ull >> 32;
    ptr[4] = ull >> 24;
    ptr[5] = ull >> 16;
    ptr[6] = ull >> 8;
    ptr[7] = ull >> 0;

    return ret;
}

uint64_t
hton64(uint64_t ull)
{
    return ntoh64(ull);
}
