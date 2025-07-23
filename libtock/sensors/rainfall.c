#include "rainfall.h"

static void rainfall_upcall(int status,
                            int rainfall,
                            __attribute__ ((unused)) int unused2, void* opaque) {
  libtock_rainfall_callback cb = (libtock_rainfall_callback) opaque;
  cb(status, rainfall);
}

returncode_t libtock_rainfall_read(libtock_rainfall_callback cb, int hours) {
  returncode_t err;

  err = libtock_rainfall_set_upcall(rainfall_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_rainfall_command_read(hours);
  return err;
}
