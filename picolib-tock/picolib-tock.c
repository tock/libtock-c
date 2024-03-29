#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <tock.h>
#include <tock_sys.h>

// Suppress missing prototype warnings for this file as the headers should be in
// picolib internals.
#pragma GCC diagnostic ignored "-Wmissing-prototypes"

int __errno_global;
int* __errno(void) { return &__errno_global; }

// FOR PICOLIB
int fstat(int fd, struct stat *st) { return _tock_fstat(fd, st); }
int isatty(int fd) { return _tock_isatty(fd); }
int read(int fd, void *buf, uint32_t count) { return _tock_read(fd, buf, count); }
int write(int fd, const void *buf, uint32_t count) { return _tock_write(fd, buf, count); }
int lseek(int fd, uint32_t offset, int whence) { return _tock_lseek(fd, offset, whence); }
int close(int fd) { return _tock_close(fd); }
caddr_t sbrk(int incr) { return _tock_sbrk(incr); }
