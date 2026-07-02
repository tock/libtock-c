#include "sdcard_syscalls.h"

returncode_t libtocksync_sdcard_yield_wait_for_initialize(uint32_t* block_size, uint32_t* size_in_kB) {
  yield_waitfor_return_t ywf;
  ywf = yield_wait_for(DRIVER_NUM_SDCARD, 0);

  // callback_type=4 indicates an error
  if (ywf.data0 == 4) return RETURNCODE_FAIL;

  if (block_size != NULL) {
    *block_size = (uint32_t) ywf.data1;
  }
  if (size_in_kB != NULL) {
    *size_in_kB = (uint32_t) ywf.data2;
  }
  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_sdcard_yield_wait_for_operation(void) {
  yield_waitfor_return_t ywf;
  ywf = yield_wait_for(DRIVER_NUM_SDCARD, 0);

  // callback_type=4 indicates an error
  if (ywf.data0 == 4) return RETURNCODE_FAIL;

  return RETURNCODE_SUCCESS;
}
