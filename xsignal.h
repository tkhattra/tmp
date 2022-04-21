#ifndef XSIGNAL_H
#define XSIGNAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <signal.h>

extern int  sig_catch(int signum, void handler(int signum));
extern void sig_pause(void);
extern int  sig_block(int signum);
extern int  sig_unblock(int signum);

#if defined(__cplusplus)
}
#endif

#endif
