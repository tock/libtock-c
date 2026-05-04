#include "nonvolatile_storage_syscalls.h"

returncode_t libtocksync_nonvolatile_storage_yield_wait_for_read(int* length) {
  yield_waitfor_return_t ret;
  ret     = yield_wait_for(DRIVER_NUM_NONVOLATILE_STORAGE, 0);
  *length = (int) ret.data0;
  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_nonvolatile_storage_yield_wait_for_write(int* length) {
  yield_waitfor_return_t ret;
  ret     = yield_wait_for(DRIVER_NUM_NONVOLATILE_STORAGE, 1);
  *length = (int) ret.data0;
  return RETURNCODE_SUCCESS;
}
