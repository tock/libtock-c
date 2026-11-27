#include "pwm_syscalls.h"

bool libtock_pwm_driver_exists(void) {
  return driver_exists(DRIVER_NUM_PWM);
}

returncode_t libtock_pwm_command_start(uint16_t pin, uint16_t duty_cycle, uint32_t frequency_hz) {
  syscall_return_t cval = command(DRIVER_NUM_PWM, 1, PWM_PIN_DUTYCYCLE(pin, duty_cycle), frequency_hz);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_pwm_command_stop(uint16_t pin) {
  syscall_return_t cval = command(DRIVER_NUM_PWM, 2, pin, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_pwm_command_get_max_frequency(uint16_t pin, uint32_t* frequency_hz) {
  syscall_return_t cval = command(DRIVER_NUM_PWM, 3, pin, 0);
  return tock_command_return_u32_to_returncode(cval, frequency_hz);
}

returncode_t libtock_pwm_command_get_count(uint32_t* count) {
  syscall_return_t cval = command(DRIVER_NUM_PWM, 4, 0, 0);
  return tock_command_return_u32_to_returncode(cval, count);
}
