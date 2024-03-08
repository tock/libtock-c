#include "pressure.h"

static void pressure_upcall(int                          pressure,
                            __attribute__ ((unused)) int unused,
                            __attribute__ ((unused)) int unused1,
                            void*                        opaque) {
  libtock_pressure_callback cb = (libtock_pressure_callback) opaque;
  cb(RETURNCODE_SUCCESS, pressure);
}

returncode_t libtock_pressure_read(libtock_pressure_callback cb) {
  returncode_t err;

  err = libtock_pressure_set_upcall(pressure_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_pressure_command_read();
  return err;
}
