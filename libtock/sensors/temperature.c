#include "temperature.h"

// Internal upcall for passing to the syscall driver.
static void temperature_upcall(int                          temp,
                               __attribute__ ((unused)) int unused,
                               __attribute__ ((unused)) int unused1,
                               void*                        opaque) {
  libtock_temperature_callback cb = (libtock_temperature_callback) opaque;
  cb(RETURNCODE_SUCCESS, temp);
}

returncode_t libtock_temperature_read(libtock_temperature_callback cb) {
  returncode_t ret;

  ret = libtock_temperature_set_upcall(temperature_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_temperature_command_read();
  return ret;
}
