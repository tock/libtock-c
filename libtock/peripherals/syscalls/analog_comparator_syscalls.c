#include "analog_comparator_syscalls.h"

bool libtock_analog_comparator_exists(void) {
  return driver_exists(DRIVER_NUM_ANALOG_COMPARATOR);
}

returncode_t libtock_analog_comparator_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_ANALOG_COMPARATOR, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_analog_comparator_command_comparison(uint8_t channel, uint32_t* comparison) {
  syscall_return_t cval = command(DRIVER_NUM_ANALOG_COMPARATOR, 1, channel, 0);
  return tock_command_return_u32_to_returncode(cval, comparison);
}

returncode_t libtock_analog_comparator_command_start_comparing(uint8_t channel) {
  syscall_return_t cval = command(DRIVER_NUM_ANALOG_COMPARATOR, 2, channel, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_analog_comparator_command_stop_comparing(uint8_t channel) {
  syscall_return_t cval = command(DRIVER_NUM_ANALOG_COMPARATOR, 3, channel, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_analog_comparator_command_count(uint32_t* count) {
  syscall_return_t cval = command(DRIVER_NUM_ANALOG_COMPARATOR, 4, 0, 0);
  return tock_command_return_u32_to_returncode(cval, count);
}
