#include "dac.h"
#include "syscalls/dac_syscalls.h"

returncode_t libtock_dac_initialize(void) {
  return libtock_dac_command_initialize();
}

returncode_t libtock_dac_set_value(uint32_t value) {
  return libtock_dac_command_set_value(value);
}
