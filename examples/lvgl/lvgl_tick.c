#include <libtock/peripherals/syscalls/alarm_syscalls.h>

#include "lvgl_tick.h"

uint32_t lvgl_tick_ms(void) {
  static bool initialized = false;
  static uint32_t frequency;
  static uint32_t last_ticks;
  static uint64_t elapsed_ticks;

  uint32_t ticks;
  libtock_alarm_command_read(&ticks);

  if (!initialized) {
    libtock_alarm_command_get_frequency(&frequency);
    last_ticks  = ticks;
    initialized = true;
  }

  // Unsigned subtraction handles the counter wrapping (once) since the last
  // call.
  elapsed_ticks += (uint32_t)(ticks - last_ticks);
  last_ticks     = ticks;

  return (uint32_t)(elapsed_ticks * 1000 / frequency);
}
