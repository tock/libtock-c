#include "servo_syscalls.h"
#include <stdio.h>

bool libtock_servo_exists(void) {
  return driver_exists(DRIVER_NUM_SERVO);
}

returncode_t libtock_servo_angle(uint16_t index, uint16_t angle) {
  syscall_return_t cval = command(DRIVER_NUM_SERVO, 1, index, angle);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_current_servo_angle(uint16_t index, uint16_t* angle) {
  uint32_t value     = 0;
  syscall_return_t r = command(DRIVER_NUM_SERVO, 2, index, 0);
  // Converts the value returned by the servo, stored in `r`,
  // and assigns it to `ret`.
  returncode_t ret = tock_command_return_u32_to_returncode(r, &value);
  *angle = (uint16_t)value;
  return ret;
}
