#include "humidity.h"
#include "syscalls/humidity_syscalls.h"
#include "tock.h"

static void humidity_upcall(int humidity,
                            __attribute__ ((unused)) int unused1,
                            __attribute__ ((unused)) int unused2, void* opaque) {
  libtock_humidity_callback cb = (libtock_humidity_callback) opaque;
  cb(RETURNCODE_SUCCESS, humidity);
}


returncode_t libtock_humidity_read(libtock_humidity_callback cb) {
  returncode_t err;

  err = libtock_humidity_set_upcall(humidity_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_humidity_command_read();
  return err;
}
