#include "dac.h"
#include "tock.h"

syscall_return_t dac_initialize(void) {
  return command(DRIVER_NUM_DAC, 1, 0, 0);
}

syscall_return_t dac_set_value(uint32_t value) {
  return command(DRIVER_NUM_DAC, 2, value, 0);
}
