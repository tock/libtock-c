#include "sha_syscalls.h"

returncode_t libtocksync_sha_yield_wait_for(void) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_SHA, 0);

  return (returncode_t) ret.data0;
}
