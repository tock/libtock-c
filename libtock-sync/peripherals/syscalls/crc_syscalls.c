#include "crc_syscalls.h"

returncode_t libtocksync_crc_yield_wait_for(uint32_t* crc) {
  yield_waitfor_return_t ywf;
  returncode_t ret;
  ywf = yield_wait_for(DRIVER_NUM_CRC, 0);
  ret = (returncode_t) ywf.data0;
  if (ret != RETURNCODE_SUCCESS) return ret;
  *crc = (uint32_t) ywf.data1;
  return ret;
}
