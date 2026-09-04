#include "pressure.h"

#include "syscalls/pressure_syscalls.h"

static void pressure_upcall(int                          status,
                            int                          pressure,
                            __attribute__ ((unused)) int unused,
                            void*                        opaque) {
  libtock_pressure_callback cb = (libtock_pressure_callback) opaque;
  cb(tock_status_to_returncode(status), pressure);
}

bool libtock_pressure_exists(void) {
  return libtock_pressure_driver_exists();
}

returncode_t libtock_pressure_read(libtock_pressure_callback cb) {
  returncode_t err;

  err = libtock_pressure_set_upcall(pressure_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_pressure_command_read();
  return err;
}
