#include "sha_syscalls.h"

returncode_t libtocksync_sha_yield_wait_for_hash(void) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_SHA, 0);

  return tock_status_to_returncode(ret.data0);
}

returncode_t libtocksync_sha_yield_wait_for_verify(bool* correct) {
  yield_waitfor_return_t ywf;
  returncode_t ret;
  ywf = yield_wait_for(DRIVER_NUM_SHA, 1);
  ret = tock_status_to_returncode(ywf.data0);
  if (ret != RETURNCODE_SUCCESS) return ret;
  *correct = ywf.data1 == 1;
  return ret;
}
