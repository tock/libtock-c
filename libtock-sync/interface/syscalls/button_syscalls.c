#include "button_syscalls.h"

returncode_t libtocksync_button_yield_wait_for(int* button_index, bool* pressed) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_BUTTON, 0);

  *button_index = (int) ret.data0;
  *pressed = ret.data1 == 1;

  return RETURNCODE_SUCCESS;
}
