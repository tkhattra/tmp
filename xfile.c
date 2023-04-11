#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <errno.h>
#include "ct_assert.h"
#include "xfile.h"

// instead of #define _GNU_SOURCE/_BSD_SOURCE/_SVID_SOURCE and #include <stdio.h>
#undef fwrite_unlocked
extern size_t fwrite_unlocked(const void *ptr, size_t size, size_t n, FILE *stream);

int
xopen(const char *pathname, int flags, mode_t mode)
{
    int fd;

    do
        fd = open(pathname, flags, mode);
    while ((fd < 0) && (errno == EINTR));
    return fd;
}

int xopen_append(const char *pathname)      { return xopen(pathname, O_WRONLY|O_CREAT|O_APPEND, 0644); }
int xopen_read(const char *pathname)        { return xopen(pathname, O_RDONLY, 0); }
int xopen_write(const char *pathname)       { return xopen(pathname, O_WRONLY, 0644); }
int xopen_trunc(const char *pathname)       { return xopen(pathname, O_WRONLY|O_CREAT|O_TRUNC, 0644); }
int xopen_readwrite(const char *pathname)   { return xopen(pathname, O_RDWR|O_CREAT, 0644); }

// like read(2), but ignore EINTR
ssize_t
xread(int fd, void *buf, size_t count)
{
    ssize_t nread;

    for (;;) {
        nread = read(fd, buf, count);
        if ((nread < 0) && (errno == EINTR))
            continue;
        break;
    }
    return nread;
}

// like pread(2), but ignore EINTR
ssize_t
xpread(int fd, void *buf, size_t count, off_t offset)
{
    ssize_t nread;

    for (;;) {
        nread = pread(fd, buf, count, offset);
        if ((nread < 0) && (errno == EINTR))
            continue;
        break;
    }
    return nread;
}

// like write(2), but ignore EINTR
ssize_t
xwrite(int fd, const void *buf, size_t count)
{
    ssize_t nwrite;

    for (;;) {
        nwrite = write(fd, buf, count);
        if ((nwrite < 0) && (errno == EINTR))
            continue;
        break;
    }
    return nwrite;
}

// like pwrite(2), but ignore EINTR
ssize_t
xpwrite(int fd, const void *buf, size_t count, off_t offset)
{
    ssize_t nwrite;

    for (;;) {
        nwrite = pwrite(fd, buf, count, offset);
        if ((nwrite < 0) && (errno == EINTR))
            continue;
        break;
    }
    return nwrite;
}

ssize_t
xreadall(int fd, void *buf, size_t count)
{
    char *bufp = buf;
    size_t nleft = count;
    ssize_t nread;

    while (nleft > 0) {
        nread = xread(fd, bufp, nleft);
        if (nread < 0)
            return -1;
        if (nread == 0)
            break;
        bufp += nread;
        nleft -= nread;
    }
    return count-nleft;
}

ssize_t
xpreadall(int fd, void *buf, size_t count, off_t offset)
{
    char *bufp = buf;
    size_t nleft = count;
    ssize_t nread;

    // use 64-bit file offsets (even on 32-bit builds)
    ct_assert(sizeof(off_t) == 8);

    while (nleft > 0) {
        nread = xpread(fd, bufp, nleft, offset);
        if (nread < 0)
            return -1;
        if (nread == 0)
            break;
        bufp += nread;
        offset += nread;
        nleft -= nread;
    }
    return count-nleft;
}

size_t
xwriteall(int fd, const void *buf, size_t count)
{
    const char *bufp = buf;
    size_t nleft = count;
    ssize_t nwrite;

    while (nleft > 0) {
        nwrite = xwrite(fd, bufp, nleft);
        if (nwrite < 0)
            return count-nleft;
        nleft -= nwrite;
        bufp += nwrite;
    }
    return count;
}

size_t
xpwriteall(int fd, const void *buf, size_t count, off_t offset)
{
    const char *bufp = buf;
    size_t nleft = count;
    ssize_t nwrite;

    // use 64-bit file offsets (even on 32-bit builds)
    ct_assert(sizeof(off_t) == 8);

    while (nleft > 0) {
        nwrite = xpwrite(fd, bufp, nleft, offset);
        if (nwrite < 0)
            return count-nleft;
        nleft -= nwrite;
        bufp += nwrite;
        offset += nwrite;
    }
    return count;
}

size_t
xfwriteall(void *fp, const void *buf, size_t count)
{
    const char *bufp = buf;
    size_t nleft = count, nwrite;

    while (nleft > 0) {
        nwrite = fwrite_unlocked(bufp, 1, nleft, fp);
        if (nwrite != nleft) {
            if (errno != EINTR) {
                nleft -= nwrite;
                return count-nleft;
            }
        }
        nleft -= nwrite;
        bufp += nwrite;
    }
    return count;
}

int
xfsync(int fd)
{
    int ret;

    do
        ret = fsync(fd);
    while ((ret < 0) && (errno == EINTR));
    return ret;
}

int
xfdatasync(int fd)
{
    int ret;

    do
        ret = fdatasync(fd);
    while ((ret < 0) && (errno == EINTR));
    return ret;
}

int
xftruncate(int fd, off_t length)
{
    int ret;

    do
        ret = ftruncate(fd, length);
    while ((ret < 0) && (errno == EINTR));
    return ret;
}

int
xtruncate(const char *pathname, off_t length)
{
    int ret;

    do
        ret = truncate(pathname, length);
    while ((ret < 0) && (errno == EINTR));
    return ret;
}

int
xndelay_on(int fd)
{
    int ret;

    ret = fcntl(fd, F_GETFL);
    return ret < 0 ? ret : fcntl(fd, F_SETFL, ret|O_NONBLOCK);
}

int
xflock_exnb(int fd)
{
    int ret;

    do
        ret = flock(fd, LOCK_EX|LOCK_NB);
    while ((ret < 0) && (errno == EINTR));
    return ret;
}
