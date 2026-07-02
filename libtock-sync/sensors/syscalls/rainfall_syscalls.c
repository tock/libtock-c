#include "rainfall_syscalls.h"

returncode_t libtocksync_rainfall_yield_wait_for(uint32_t* rainfall) {
  yield_waitfor_return_t ywf;
  returncode_t ret;
  ywf = yield_wait_for(DRIVER_NUM_RAINFALL, 0);
  ret = (returncode_t) ywf.data0;
  if (ret != RETURNCODE_SUCCESS) return ret;
  *rainfall = (uint32_t) ywf.data1;
  return ret;
}
