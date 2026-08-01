#include "hmac_syscalls.h"

returncode_t libtocksync_hmac_yield_wait_for(void) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_HMAC, 0);

  return tock_status_to_returncode(ret.data0);
}
