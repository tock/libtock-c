#include "console_syscalls.h"

returncode_t libtocksync_console_yield_wait_for_write(uint32_t* bytes_written) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_CONSOLE, 1);

  *bytes_written = ret.data1;

  return (returncode_t) ret.data0;
}

returncode_t libtocksync_console_yield_wait_for_read(uint32_t* bytes_read) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_CONSOLE, 2);

  *bytes_read = ret.data1;

  return (returncode_t) ret.data0;
}
