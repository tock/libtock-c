#pragma once

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

int _tock_close(int fd);
int _tock_lseek(int fd, uint32_t offset, int whence);
int _tock_write(int fd, const void *buf, uint32_t count);
int _tock_read(int fd, void *buf, uint32_t count);
int _tock_isatty(int fd);
int _tock_fstat(int fd, struct stat *st);
caddr_t _tock_sbrk(int incr);

#ifdef __cplusplus
}
#endif
