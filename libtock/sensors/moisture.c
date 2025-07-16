#include "moisture.h"

#include "syscalls/moisture_syscalls.h"

static void moisture_upcall(int status,
                            int moisture,
                            __attribute__ ((unused)) int unused2, void* opaque) {
  libtock_moisture_callback cb = (libtock_moisture_callback) opaque;
  cb(tock_status_to_returncode(status), moisture);
}

bool libtock_moisture_exists(void) {
  return libtock_moisture_driver_exists();
}

returncode_t libtock_moisture_read(libtock_moisture_callback cb) {
  returncode_t err;

  err = libtock_moisture_set_upcall(moisture_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_moisture_command_read();
  return err;
}
