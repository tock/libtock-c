#include "screen_syscalls.h"

returncode_t libtocksync_screen_yield_wait_for(void) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_SCREEN, 0);

  int status = tock_status_to_returncode((statuscode_t) ret.data0);
  return (returncode_t) status;
}

returncode_t libtocksync_screen_yield_wait_for_format(libtock_screen_format_t* format) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_SCREEN, 0);

  *format = (libtock_screen_format_t) ret.data1;

  int status = tock_status_to_returncode((statuscode_t) ret.data0);
  return (returncode_t) status;
}

returncode_t libtocksync_screen_yield_wait_for_rotation(libtock_screen_rotation_t* rotation) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_SCREEN, 0);

  *rotation = (libtock_screen_rotation_t) ret.data1;

  int status = tock_status_to_returncode((statuscode_t) ret.data0);
  return (returncode_t) status;
}
