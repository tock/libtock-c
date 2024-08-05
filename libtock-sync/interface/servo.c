#include "servo.h"
#include "../../libtock/interface/syscalls/servo_syscalls.h"

returncode_t libtocksync_servo_angle(uint32_t angle) {
  int err;
  err = libtock_servo_angle(angle);
  if (err != RETURNCODE_SUCCESS) return err;

  return RETURNCODE_SUCCESS;
}
