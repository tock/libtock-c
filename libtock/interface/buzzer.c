#include "tock.h"

#include "buzzer.h"
#include "buzzer_syscalls.h"




static void buzzer_temp_upcall(__attribute__ ((unused)) int                            arg0,
                            __attribute__ ((unused)) int                            arg1,
                            __attribute__ ((unused)) int   arg2,
                            void *opaque) {
  buzzer_done_callback cb = (buzzer_done_callback) opaque;
  cb();
}

returncode_t buzzer_tone(uint32_t frequency_hz, uint32_t duration_ms, buzzer_done_callback cb) {

  returncode_t ret;

  ret = buzzer_set_upcall(buzzer_temp_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = buzzer_command_tone(frequency_hz, duration_ms);
  return ret;
}
