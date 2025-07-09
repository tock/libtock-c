#include "temperature_syscalls.h"

returncode_t libtocksync_temperature_yield_wait_for(int* temp) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_TEMPERATURE, 0);

  *temp = (int) ret.data0;
  return RETURNCODE_SUCCESS;
}
