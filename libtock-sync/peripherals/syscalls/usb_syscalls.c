#include "usb_syscalls.h"

returncode_t libtocksync_usb_yield_wait_for(void) {
  yield_waitfor_return_t ywf;
  ywf = yield_wait_for(DRIVER_NUM_USB, 0);
  return (returncode_t) ywf.data0;
}
