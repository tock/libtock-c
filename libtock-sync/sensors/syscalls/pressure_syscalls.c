#include "pressure_syscalls.h"

returncode_t libtocksync_pressure_yield_wait_for(int* pressure) {
  yield_waitfor_return_t ywf;
  returncode_t ret;
  ywf = yield_wait_for(DRIVER_NUM_PRESSURE, 0);
  ret = tock_status_to_returncode(ywf.data0);
  if (ret != RETURNCODE_SUCCESS) return ret;
  *pressure = (uint32_t) ywf.data1;
  return ret;
}
