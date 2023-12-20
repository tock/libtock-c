#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "console.h"
#include "tock.h"
#include "tock_sys.h"

// XXX Suppress unused parameter warnings for this file as the implementations
// are currently all just stubs
#pragma GCC diagnostic ignored "-Wunused-parameter"

// XXX Suppress missing prototype warnings for this file as the headers should
// be in newlib internals, but first stab at including things didn't quite work
// and the warnings are just noise
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma GCC diagnostic ignored "-Wstrict-prototypes"

// XXX Also suppress attribute suggestions as these are stubs
#pragma GCC diagnostic ignored "-Wsuggest-attribute=const"

// ------------------------------
// LIBC SUPPORT STUBS
// ------------------------------

void* __dso_handle = 0;

int _unlink(const char *pathname) {
  return -1;
}
int _open(const char* path, int flags, ...) {
  return -1;
}
void _exit(int __status) {
  tock_exit((uint32_t) __status);
}
int _getpid(void) {
  return 0;
}
int _kill(pid_t pid, int sig) {
  return -1;
}

// FOR NEWLIB
int _fstat(int fd, struct stat *st) {
  return _tock_fstat(fd, st);
}
int _isatty(int fd) {
  return _tock_isatty(fd);
}
int _read(int fd, void *buf, uint32_t count) {
  return _tock_read(fd, buf, count);
}
int _write(int fd, const void *buf, uint32_t count) {
  return _tock_write(fd, buf, count);
}
int _lseek(int fd, uint32_t offset, int whence) {
  return _tock_lseek(fd, offset, whence);
}
int _close(int fd) {
  return _tock_close(fd);
}
caddr_t _sbrk(int incr) {
  return _tock_sbrk(incr);
}
