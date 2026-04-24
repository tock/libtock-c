#include "ambient_light_syscalls.h"

returncode_t libtocksync_ambient_light_yield_wait_for(int* intensity) {
  yield_waitfor_return_t ret;
  ret        = yield_wait_for(DRIVER_NUM_AMBIENT_LIGHT, 0);
  *intensity = (int) ret.data0;
  return RETURNCODE_SUCCESS;
}
