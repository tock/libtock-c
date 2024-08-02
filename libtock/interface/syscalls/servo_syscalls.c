#include "servo_syscalls.h"
#include <stdio.h>

bool libtock_servo_exists(void) {
  return driver_exists(DRIVER_NUM_SERVO);
}

returncode_t libtock_servo_angle(uint32_t angle) {
  syscall_return_t cval = command(DRIVER_NUM_SERVO, 1, angle, 0);
  return tock_command_return_novalue_to_returncode(cval);
}