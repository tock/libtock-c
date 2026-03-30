#include "ninedof_syscalls.h"

returncode_t libtocksync_ninedof_yield_wait_for(int* x, int* y, int* z) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_NINEDOF, 0);
  *x  = (int) ret.data0;
  *y  = (int) ret.data1;
  *z  = (int) ret.data2;
  return RETURNCODE_SUCCESS;
}
