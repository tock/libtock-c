#include "buzzer.h"

returncode_t libtocksync_buzzer_tone(uint32_t frequency_hz, uint32_t duration_ms) {
  returncode_t ret;

  ret = libtock_buzzer_command_tone(frequency_hz,  duration_ms);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for the callback meaning the tone is finished.
  libtocksync_buzzer_yield_wait_for();
  return RETURNCODE_SUCCESS;
}
