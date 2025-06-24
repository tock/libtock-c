#include "servo_syscalls.h"
#include <stdio.h>

bool libtock_servo_exists(void) {
  return driver_exists(DRIVER_NUM_SERVO);
}

returncode_t libtock_servo_count(uint32_t* servo_count) {
  uint32_t value     = 0;
  syscall_return_t r = command(DRIVER_NUM_SERVO, 1, 0, 0);
  returncode_t ret   = tock_command_return_u32_to_returncode(r, &value);
  *servo_count = (uint16_t)value;
  return ret;
}

returncode_t libtock_servo_set_angle(uint16_t index, uint16_t angle) {
  syscall_return_t cval = command(DRIVER_NUM_SERVO, 2, index, angle);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_servo_read_angle(uint16_t index, uint16_t* angle) {
  uint32_t value     = 0;
  syscall_return_t r = command(DRIVER_NUM_SERVO, 3, index, 0);
  returncode_t ret   = tock_command_return_u32_to_returncode(r, &value);
  *angle = (uint16_t)value;
  return ret;
}
