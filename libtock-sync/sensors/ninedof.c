#include <math.h>

#include "ninedof.h"

#include "syscalls/ninedof_syscalls.h"

bool libtocksync_ninedof_exists(void) {
  return libtock_ninedof_driver_exists();
}

returncode_t libtocksync_ninedof_read_accelerometer(int* x, int* y, int* z) {
  returncode_t err;

  err = libtock_ninedof_command_start_accelerometer_reading();
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_ninedof_yield_wait_for(x, y, z);
  return err;
}

returncode_t libtocksync_ninedof_read_accelerometer_magnitude(double* magnitude) {
  returncode_t err;
  int x, y, z;

  err = libtocksync_ninedof_read_accelerometer(&x, &y, &z);
  if (err != RETURNCODE_SUCCESS) return err;

  *magnitude = sqrt(x * x + y * y + z * z);

  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_ninedof_read_magnetometer(int* x, int* y, int* z) {
  returncode_t err;

  err = libtock_ninedof_command_start_magnetometer_reading();
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_ninedof_yield_wait_for(x, y, z);
  return err;
}

returncode_t libtocksync_ninedof_read_gyroscope(int* x, int* y, int* z) {
  returncode_t err;

  err = libtock_ninedof_command_start_gyroscope_reading();
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_ninedof_yield_wait_for(x, y, z);
  return err;
}
