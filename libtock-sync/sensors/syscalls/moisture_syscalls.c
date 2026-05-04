#include "moisture_syscalls.h"

returncode_t libtocksync_moisture_yield_wait_for(int* moisture) {
  yield_waitfor_return_t ywf;
  returncode_t ret;
  ywf = yield_wait_for(DRIVER_NUM_MOISTURE, 0);
  ret = (returncode_t) ywf.data0;
  if (ret != RETURNCODE_SUCCESS) return ret;
  *moisture = (int) ywf.data1;
  return ret;
}
