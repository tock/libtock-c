#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "console.h"
#include "tock.h"

// XXX Suppress unused parameter warnings for this file as the implementations
// are currently all just stubs
#pragma GCC diagnostic ignored "-Wunused-parameter"

// XXX Suppress missing prototype warnings for this file as the headers should
// be in newlib internals, but first stab at including things didn't quite work
// and the warnings are just noise
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma GCC diagnostic ignored "-Wstrict-prototypes"

int _tock_close(int fd) {
  return -1;
}

int _tock_lseek(int fd, uint32_t offset, int whence) {
  return 0;
}

int _tock_write(int fd, const void *buf, uint32_t count) {
  putnstr((const char*)buf, count);
  return count;
}

int _tock_read(int fd, void *buf, uint32_t count) {
  return 0;
}

int _tock_isatty(int fd) {
  if (fd == 0) {
    return 1;
  }
  return 0;
}

int _tock_fstat(int fd, struct stat *st) {
  st->st_mode = S_IFCHR;
  return 0;
}

caddr_t _tock_sbrk(int incr) {
  memop_return_t ret;
  ret = memop(1, incr);
  if (ret.status != TOCK_STATUSCODE_SUCCESS) {
    errno = ENOMEM;
    return (caddr_t) -1;
  }
  return (caddr_t) ret.data;
}
