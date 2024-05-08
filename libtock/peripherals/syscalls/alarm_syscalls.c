#include "alarm_syscalls.h"

bool libtock_alarm_exists(void) {
  return driver_exists(DRIVER_NUM_ALARM);
}

int libtock_alarm_set_upcall(subscribe_upcall callback, void *opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_ALARM, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

int libtock_alarm_command_get_frequency(uint32_t* frequency) {
  syscall_return_t rval = command(DRIVER_NUM_ALARM, 1, 0, 0);
  return tock_command_return_u32_to_returncode(rval, frequency);
}

int libtock_alarm_command_read(uint32_t* time) {
  syscall_return_t rval = command(DRIVER_NUM_ALARM, 2, 0, 0);
  return tock_command_return_u32_to_returncode(rval, time);
}

int libtock_alarm_command_stop(void) {
  syscall_return_t rval = command(DRIVER_NUM_ALARM, 3, 0, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int libtock_alarm_command_set_relative(uint32_t dt, uint32_t* actual) {
  syscall_return_t rval = command(DRIVER_NUM_ALARM, 5, dt, 0);
  return tock_command_return_u32_to_returncode(rval, actual);
}

int libtock_alarm_command_set_absolute(uint32_t reference, uint32_t dt) {
  syscall_return_t rval = command(DRIVER_NUM_ALARM, 6, reference, dt);
  uint32_t unused;
  return tock_command_return_u32_to_returncode(rval, &unused);
}
