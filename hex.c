#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include "hex.h"

int
hexvalid(const char *hexbuf, int len)
{
    int i;

    for (i=0; i < len; i++)
        if (!isxdigit(hexbuf[i]))
            return 0;
    return 1;
}

void
hexparse(uint8_t *buf, const char *hexbuf, int len)
{
    int i;
    char tmp[3] = {0};

    for (i=0; i < len; i++) {
        tmp[0] = hexbuf[0];
        tmp[1] = hexbuf[1];
        *buf++ = strtoul(tmp, NULL, 16);
        hexbuf += 2;
    }
}

void
hexfmt(char *obuf, int olen, const char *ibuf, int ilen)
{
    int i;

    if (olen <= 0)
        return;

    // ensure that we don't overflow obuf
    if (ilen >= olen/3)
        ilen = (olen-1)/3;

    for (i=0; i < ilen; i++)
        snprintf(obuf + i*3, 4, "%02x ", ibuf[i]);
    obuf[i*3] = 0;
}
