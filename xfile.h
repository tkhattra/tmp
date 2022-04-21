#ifndef XFILE_H
#define XFILE_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <sys/types.h>

extern int      xopen(const char *pathname, int flags, mode_t mode);
extern int      xopen_append(const char *pathname);
extern int      xopen_read(const char *pathname);
extern int      xopen_write(const char *pathname);
extern int      xopen_trunc(const char *pathname);
extern int      xopen_readwrite(const char *pathname);

extern ssize_t  xwrite(int fd, const void *buf, size_t count);
extern ssize_t  xpwrite(int fd, const void *buf, size_t count, off_t offset);
extern ssize_t  xread(int fd, void *buf, size_t count);
extern ssize_t  xpread(int fd, void *buf, size_t count, off_t offset);

extern size_t   xwriteall(int fd, const void *buf, size_t count);
extern size_t   xpwriteall(int fd, const void *buf, size_t count, off_t offset);
extern ssize_t  xreadall(int fd, void *buf, size_t count);
extern ssize_t  xpreadall(int fd, void *buf, size_t count, off_t offset);
extern size_t   xfwriteall(void *fp, const void *buf, size_t count);

extern int      xfsync(int fd);
extern int      xfdatasync(int fd);
extern int      xftruncate(int fd, off_t length);
extern int      xtruncate(const char *pathname, off_t length);

extern int      xndelay_on(int fd);
extern int      xflock_exnb(int fd);

#if defined(__cplusplus)
}
#endif

#endif
