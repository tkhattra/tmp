#ifndef HEX_H
#define HEX_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

extern int  hexvalid(const char *hexbuf, int len);
extern void hexparse(uint8_t *buf, const char *hexbuf, int len);
extern void hexfmt(char *obuf, int olen, const char *ibuf, int ilen);

#if defined(__cplusplus)
}
#endif

#endif
