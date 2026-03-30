#include "kv_syscalls.h"

returncode_t libtocksync_kv_yield_wait_for_get(int* length) {
  yield_waitfor_return_t ywf;
  returncode_t ret;
  ywf = yield_wait_for(DRIVER_NUM_KV, 0);
  ret = (returncode_t) ywf.data0;
  if (ret != RETURNCODE_SUCCESS) return ret;
  *length = (int) ywf.data1;
  return ret;
}

returncode_t libtocksync_kv_yield_wait_for_done(void) {
  yield_waitfor_return_t ywf;
  ywf = yield_wait_for(DRIVER_NUM_KV, 0);
  return (returncode_t) ywf.data0;
}
