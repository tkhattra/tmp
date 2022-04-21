#include "xsignal.h"

void
sig_pause(void)
{
    sigset_t ss;

    sigemptyset(&ss);
    sigsuspend(&ss);
}

int
sig_catch(int signum, void handler(int signum))
{
    struct sigaction sa;

    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    return sigaction(signum, &sa, 0);
}

// note: uses pthread_sigmask, not sigprocmask
int
sig_block(int signum)
{
    sigset_t ss;

    sigemptyset(&ss);
    sigaddset(&ss, signum);
    return pthread_sigmask(SIG_BLOCK, &ss, 0);
}

// note: uses pthread_sigmask, not sigprocmask
int
sig_unblock(int signum)
{
    sigset_t ss;

    sigemptyset(&ss);
    sigaddset(&ss, signum);
    return pthread_sigmask(SIG_UNBLOCK, &ss, 0);
}
