#include <interface/buzzer.h>

#include "buzzer.h"

struct data {
  bool fired;
};

static struct data result = { .fired = false };


static void buzzer_cb(void) {
  result.fired = true;
}


returncode_t libtocksync_buzzer_tone(uint32_t frequency_hz, uint32_t duration_ms) {
  int err;
  result.fired = false;

  err = buzzer_tone(frequency_hz,  duration_ms, buzzer_cb);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback meaning the tone is finished.
  yield_for(&result.fired);
  return RETURNCODE_SUCCESS;
}