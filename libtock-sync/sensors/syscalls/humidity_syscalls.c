#include "humidity_syscalls.h"

returncode_t libtocksync_humidity_yield_wait_for(int* humidity) {
  yield_waitfor_return_t ret;
  ret       = yield_wait_for(DRIVER_NUM_HUMIDITY, 0);
  *humidity = (int) ret.data0;
  return RETURNCODE_SUCCESS;
}
