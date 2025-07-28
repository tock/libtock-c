#include "analog_comparator.h"

#include "syscalls/analog_comparator_syscalls.h"

bool libtock_analog_comparator_exists(void) {
  return libtock_analog_comparator_driver_exists();
}

returncode_t libtock_analog_comparator_count(int* count) {
  return libtock_analog_comparator_command_count((uint32_t*) count);
}

returncode_t libtock_analog_comparator_comparison(uint8_t channel, bool* comparison) {
  return libtock_analog_comparator_command_comparison(channel, (uint32_t*) comparison);
}

returncode_t libtock_analog_comparator_start_comparing(uint8_t channel) {
  return libtock_analog_comparator_command_start_comparing(channel);
}

returncode_t libtock_analog_comparator_stop_comparing(uint8_t channel) {
  return libtock_analog_comparator_command_stop_comparing(channel);
}
