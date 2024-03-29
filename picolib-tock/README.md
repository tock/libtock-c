Picolib - Tock Library
======================

This library provides helper functions to connect picolib system functions to
Tock-provided functions.

Specifically, picolib expects these functions:

- `int fstat(int fd, struct stat *st)`
- `int isatty(int fd)`
- `int read(int fd, void *buf, uint32_t count)`
- `int write(int fd, const void *buf, uint32_t count)`
- `int lseek(int fd, uint32_t offset, int whence)`
- `int close(int fd)`
- `caddr_t sbrk(int incr)`

These names are different from what newlib expects. Since the underscore
versions are less likely to conflict, we include those functions in libtock
directly, and then only include this library when picolib is being used.
