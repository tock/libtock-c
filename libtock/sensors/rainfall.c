#include "rainfall.h"

#include "syscalls/rainfall_syscalls.h"

static void rainfall_upcall(int status,
                            int rainfall,
                            __attribute__ ((unused)) int unused2, void* opaque) {
  libtock_rainfall_callback cb = (libtock_rainfall_callback) opaque;
  cb(tock_status_to_returncode(status), rainfall);
}

bool libtock_rainfall_exists(void) {
  return libtock_rainfall_driver_exists();
}

returncode_t libtock_rainfall_read(libtock_rainfall_callback cb, int hours) {
  returncode_t err;

  err = libtock_rainfall_set_upcall(rainfall_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_rainfall_command_read(hours);
  return err;
}
