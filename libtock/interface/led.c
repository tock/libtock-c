#include "led.h"

#include "syscalls/led_syscalls.h"

bool libtock_led_exists(void) {
  return libtock_led_driver_exists();
}

returncode_t libtock_led_count(int* count) {
  return libtock_led_command_count(count);
}

returncode_t libtock_led_on(int led_num) {
  return libtock_led_command_on(led_num);
}

returncode_t libtock_led_off(int led_num) {
  return libtock_led_command_off(led_num);
}

returncode_t libtock_led_toggle(int led_num) {
  return libtock_led_command_toggle(led_num);
}
