#include "proximity_syscalls.h"

returncode_t libtocksync_proximity_yield_wait_for(uint8_t* proximity) {
  yield_waitfor_return_t ret;
  ret        = yield_wait_for(DRIVER_NUM_PROXIMITY, 0);
  *proximity = (uint8_t) ret.data0;
  return RETURNCODE_SUCCESS;
}
