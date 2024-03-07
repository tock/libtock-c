#include "ambient_light.h"
#include "ambient_light_syscalls.h"
#include "tock.h"



// callback for synchronous reads
static void ambient_light_upcall(int intensity,
                                 __attribute__ ((unused)) int unused1,
                                 __attribute__ ((unused)) int unused2, void* opaque) {
  libtock_ambient_light_callback cb = (libtock_ambient_light_callback) opaque;
  cb(RETURNCODE_SUCCESS, intensity);
}



returncode_t libtock_ambient_light_read_intensity(libtock_ambient_light_callback cb) {
  returncode_t err;

  err = libtock_ambient_light_set_upcall(ambient_light_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_ambient_light_command_start_intensity_reading();
  return err;
}
