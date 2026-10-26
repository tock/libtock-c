#include "pwm.h"

#include "syscalls/pwm_syscalls.h"

bool libtock_pwm_exists(void) {
  return libtock_pwm_driver_exists();
}

returncode_t libtock_pwm_start(uint16_t pin, uint16_t duty_cycle, uint32_t frequency_hz) {
  return libtock_pwm_command_start(pin, duty_cycle, frequency_hz);
}

returncode_t libtock_pwm_stop(uint16_t pin) {
  return libtock_pwm_command_stop(pin);
}

returncode_t libtock_pwm_get_max_frequency(uint16_t pin, uint32_t* frequency_hz) {
  return libtock_pwm_command_get_max_frequency(pin, frequency_hz);
}

returncode_t libtock_pwm_get_count(uint32_t* count) {
  return libtock_pwm_command_get_count(count);
}
