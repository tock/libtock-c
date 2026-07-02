#include "isolated_nonvolatile_storage_syscalls.h"

returncode_t libtocksync_isolated_nonvolatile_storage_yield_wait_for_get_number_bytes(uint64_t* number_bytes) {
  yield_waitfor_return_t ywf;
  returncode_t ret;
  ywf = yield_wait_for(DRIVER_NUM_ISOLATED_NONVOLATILE_STORAGE, 0);
  ret = (returncode_t) ywf.data0;
  if (ret != RETURNCODE_SUCCESS) return ret;
  *number_bytes = ((uint64_t) ywf.data2 << 32) | (uint64_t) ywf.data1;
  return ret;
}

returncode_t libtocksync_isolated_nonvolatile_storage_yield_wait_for_read(void) {
  yield_waitfor_return_t ywf;
  ywf = yield_wait_for(DRIVER_NUM_ISOLATED_NONVOLATILE_STORAGE, 1);
  return (returncode_t) ywf.data0;
}

returncode_t libtocksync_isolated_nonvolatile_storage_yield_wait_for_write(void) {
  yield_waitfor_return_t ywf;
  ywf = yield_wait_for(DRIVER_NUM_ISOLATED_NONVOLATILE_STORAGE, 2);
  return (returncode_t) ywf.data0;
}
