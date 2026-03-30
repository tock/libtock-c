#include "pressure_syscalls.h"

returncode_t libtocksync_pressure_yield_wait_for(int* pressure) {
  yield_waitfor_return_t ret;
  ret       = yield_wait_for(DRIVER_NUM_PRESSURE, 0);
  *pressure = (int) ret.data0;
  return RETURNCODE_SUCCESS;
}
