#ifndef XUTIL_H
#define XUTIL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <sys/types.h>

// compile-time assert
#define ct_assert(e) ((void)sizeof(char[1 - 2*!(e)]))

#ifndef NARR
#define NARR(a) (sizeof(a)/sizeof(a[0]))
#endif

#ifndef USED
#define USED(x) ((void)x)
#endif

#if defined(__cplusplus)
}
#endif

#endif
