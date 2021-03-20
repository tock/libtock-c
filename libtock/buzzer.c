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
  return command (BUZZER_DRIVER, 0, 0, 0).type == TOCK_SYSCALL_SUCCESS;
}

int tone_sync (size_t frequency_hz, size_t duration_ms) {
  bool done = false;
  subscribe_return_t sval = subscribe (BUZZER_DRIVER, 0, callback_sync, &done);
  if (!sval.success) {
    return tock_error_to_rcode(sval.error);
  }
  
  syscall_return_t cval = command (BUZZER_DRIVER, 1, frequency_hz, duration_ms);
  if (cval.type == TOCK_SYSCALL_SUCCESS) {
    yield_for (&done);
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(cval.data[0]);
  }
}

int tone (size_t frequency_hz, size_t duration_ms, void (*tone_done)(void)) {
  subscribe_return_t sval = subscribe (BUZZER_DRIVER, 0, callback, tone_done);
  if (!sval.success) {
    return tock_error_to_rcode(sval.error);
  }
  syscall_return_t cval = command (BUZZER_DRIVER, 1, frequency_hz, duration_ms);
  if (cval.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(cval.data[0]);
  }
}
