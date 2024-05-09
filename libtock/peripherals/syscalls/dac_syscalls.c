#include "dac_syscalls.h"

bool libtock_dac_exists(void) {
  return driver_exists(DRIVER_NUM_DAC);
}

returncode_t libtock_dac_command_initialize(void) {
  syscall_return_t cval = command(DRIVER_NUM_DAC, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_dac_command_set_value(uint32_t value) {
  syscall_return_t cval = command(DRIVER_NUM_DAC, 2, value, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
