#include "ninedof.h"

// internal callback for faking synchronous reads
static void ninedof_upcall(int x, int y, int z, void* opaque) {
  libtock_ninedof_callback cb = (libtock_ninedof_callback) opaque;
  cb(RETURNCODE_SUCCESS, x, y, z);
}


returncode_t libtock_ninedof_read_accelerometer(libtock_ninedof_callback cb) {
  returncode_t err;

  err = libtock_ninedof_set_upcall(ninedof_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_ninedof_command_start_accelerometer_reading();
  return err;
}

returncode_t libtock_ninedof_read_magnetometer(libtock_ninedof_callback cb) {
  returncode_t err;

  err = libtock_ninedof_set_upcall(ninedof_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_ninedof_command_start_magnetometer_reading();
  return err;
}

returncode_t libtock_ninedof_read_gyroscope(libtock_ninedof_callback cb) {
  returncode_t err;

  err = libtock_ninedof_set_upcall(ninedof_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_ninedof_command_start_gyroscope_reading();
  return err;
}
