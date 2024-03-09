#include "sound_pressure.h"
#include "syscalls/sound_pressure_syscalls.h"
#include "tock.h"


static void sound_pressure_upcall(int                          sound_pressure,
                                  __attribute__ ((unused)) int unused,
                                  __attribute__ ((unused)) int unused1,
                                  void*                        opaque) {
  libtock_sound_pressure_callback cb = (libtock_sound_pressure_callback) opaque;
  cb(RETURNCODE_SUCCESS, (uint8_t) sound_pressure);
}



returncode_t libtock_sound_pressure_read(libtock_sound_pressure_callback cb) {
  returncode_t err;

  err = libtock_sound_pressure_set_upcall(sound_pressure_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_sound_pressure_command_read();
  return err;
}
