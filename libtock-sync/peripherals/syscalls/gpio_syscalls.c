#include "gpio_syscalls.h"

returncode_t libtocksync_gpio_yield_wait_for(uint32_t* pin, bool* value) {
  yield_waitfor_return_t ret;
  ret    = yield_wait_for(GPIO_DRIVER_NUM, 0);
  *pin   = (uint32_t) ret.data0;
  *value = ret.data1 == 1;
  return RETURNCODE_SUCCESS;
}
