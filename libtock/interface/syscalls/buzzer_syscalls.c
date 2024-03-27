#include "buzzer_syscalls.h"

bool libtock_buzzer_exists(void) {
  return driver_exists(DRIVER_NUM_BUZZER);
}

returncode_t libtock_buzzer_set_upcall(subscribe_upcall callback, void* opaque){
  subscribe_return_t sval = subscribe(DRIVER_NUM_BUZZER, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_buzzer_command_tone(uint32_t frequency_hz, uint32_t duration_ms) {
  syscall_return_t cval = command(DRIVER_NUM_BUZZER, 1, frequency_hz, duration_ms);
  return tock_command_return_novalue_to_returncode(cval);
}
