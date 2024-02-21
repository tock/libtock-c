#include "led.h"
#include "led_syscalls.h"

returncode_t led_count(int* count) {
  return led_command_count(count);
}

returncode_t led_on(int led_num) {
  return led_command_on(led_num);
}

returncode_t led_off(int led_num) {
  return led_command_off(led_num);
}

returncode_t led_toggle(int led_num) {
  return led_command_toggle(led_num);
}
