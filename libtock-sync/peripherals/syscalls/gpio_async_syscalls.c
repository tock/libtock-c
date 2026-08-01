#include <libtock/peripherals/syscalls/gpio_async_syscalls.h>

#include "gpio_async_syscalls.h"

returncode_t libtocksync_gpio_async_yield_wait_for_generic_command(void) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_GPIO_ASYNC, 0);

  return tock_status_to_returncode(ret.data0);
}

returncode_t libtocksync_gpio_async_yield_wait_for_read(bool* value) {
  yield_waitfor_return_t ywf;
  returncode_t ret;

  ywf = yield_wait_for(DRIVER_NUM_GPIO_ASYNC, 0);

  ret = tock_status_to_returncode(ywf.data0);
  if (ret != RETURNCODE_SUCCESS) return ret;

  *value = (bool) ywf.data1;
  return ret;
}
