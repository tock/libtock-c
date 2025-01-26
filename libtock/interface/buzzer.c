#include "buzzer.h"

static void libtock_buzzer_temp_upcall(__attribute__ ((unused)) int arg0,
                                       __attribute__ ((unused)) int arg1,
                                       __attribute__ ((unused)) int arg2,
                                       void*                        opaque) {
  libtock_buzzer_done_callback cb = (libtock_buzzer_done_callback) opaque;
  cb();
}

returncode_t libtock_buzzer_tone(uint32_t frequency_hz, uint32_t duration_ms, libtock_buzzer_done_callback cb) {

  returncode_t ret;

  ret = libtock_buzzer_set_upcall(libtock_buzzer_temp_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_buzzer_command_tone(frequency_hz, duration_ms);
  return ret;
}
