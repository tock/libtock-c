#include "proximity.h"
#include "syscalls/proximity_syscalls.h"
#include "tock.h"

// Internal callback for faking synchronous reads
static void proximity_cb(int                         proximity,
                         __attribute__((unused)) int unused,
                         __attribute__((unused)) int unused1,
                         void *                      opaque) {
  libtock_proximity_callback cb = (libtock_proximity_callback) opaque;
  cb(RETURNCODE_SUCCESS, (uint8_t) proximity);
}

returncode_t libtock_proximity_read(libtock_proximity_callback cb) {
  returncode_t err;

  err = libtock_proximity_set_upcall(proximity_cb, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_proximity_command_read();
  return err;
}

returncode_t libtock_proximity_read_on_interrupt(uint32_t lower_threshold, uint32_t higher_threshold,
                                                 libtock_proximity_callback cb) {
  returncode_t err;

  err = libtock_proximity_set_upcall(proximity_cb, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_proximity_command_read_on_interrupt(lower_threshold, higher_threshold);
  return err;
}
