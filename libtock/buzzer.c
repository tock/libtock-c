#include "buzzer.h"
#include "tock.h"

// Internal callback for faking synchronous reads
static void callback_sync (__attribute__ ((unused)) int unused,
                           __attribute__ ((unused)) int unused1,
                           __attribute__ ((unused)) int unused2,
                           void* ud) {
  *(bool*)ud = true;
}

static void callback(__attribute__ ((unused)) int unused,
                     __attribute__ ((unused)) int unused1,
                     __attribute__ ((unused)) int unused2,
                     void* ud) {
  ((void (*)(void))ud)();
}

int buzzer_exists (void) {
  return command (BUZZER_DRIVER, 0, 0, 0);
}

int tone_sync (size_t frequency_hz, size_t duration_ms) {
  bool done = false;
  int ret   = subscribe (BUZZER_DRIVER, 0, callback_sync, &done);
  if (ret == TOCK_SUCCESS) {
    ret = command (BUZZER_DRIVER, 1, frequency_hz, duration_ms);
    if (ret == TOCK_SUCCESS) yield_for (&done);
  }
  return ret;
}
int tone (size_t frequency_hz, size_t duration_ms, void (*tone_done)(void)) {
  int ret = subscribe (BUZZER_DRIVER, 0, callback, tone_done);
  if (ret == TOCK_SUCCESS) {
    ret = command (BUZZER_DRIVER, 1, frequency_hz, duration_ms);
  }
  return ret;
}