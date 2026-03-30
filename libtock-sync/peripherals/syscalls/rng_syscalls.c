#include "rng_syscalls.h"

returncode_t libtocksync_rng_yield_wait_for(int* received) {
  yield_waitfor_return_t ret;
  ret       = yield_wait_for(DRIVER_NUM_RNG, 0);
  *received = (int) ret.data1;
  return RETURNCODE_SUCCESS;
}
