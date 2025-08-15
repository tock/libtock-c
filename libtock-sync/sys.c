#include "interface/console.h"

// XXX Suppress missing prototype warnings for this file as the headers should
// be in newlib internals, but first stab at including things didn't quite work
// and the warnings are just noise
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma GCC diagnostic ignored "-Wstrict-prototypes"

// ------------------------------
// SYNCHRONOUS LIBC SUPPORT STUBS
// ------------------------------

int _write(__attribute__ ((unused)) int fd, const void* buf, uint32_t count) {
  uint32_t written;
  libtocksync_console_write((const uint8_t*) buf, count, &written);
  return (int) written;
}
