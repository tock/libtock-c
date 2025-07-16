#include <math.h>
#include <stdio.h>

#include <libtock/sensors/syscalls/ninedof_syscalls.h>

#include "ninedof.h"

struct ninedof_data {
  int x;
  int y;
  int z;
  bool fired;
  returncode_t ret;
};

static struct ninedof_data result = { .fired = false };



static void ninedof_cb(returncode_t ret, int x, int y, int z) {
  result.x     = x;
  result.y     = y;
  result.z     = z;
  result.fired = true;
  result.ret   = ret;
}


returncode_t libtocksync_ninedof_read_accelerometer(int* x, int* y, int* z) {
  returncode_t err;

  result.fired = false;

  err = libtock_ninedof_read_accelerometer(ninedof_cb);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *x = result.x;
  *y = result.y;
  *z = result.z;

  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_ninedof_read_accelerometer_magnitude(double* magnitude) {
  returncode_t err;
  int x, y, z;

  err = libtocksync_ninedof_read_accelerometer(&x, &y, &z);
  if (err != RETURNCODE_SUCCESS) return err;

  *magnitude = sqrt(result.x * result.x + result.y * result.y + result.z * result.z);

  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_ninedof_read_magnetometer(int* x, int* y, int* z) {
  returncode_t err;

  result.fired = false;

  err = libtock_ninedof_read_magnetometer(ninedof_cb);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *x = result.x;
  *y = result.y;
  *z = result.z;

  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_ninedof_read_gyroscope(int* x, int* y, int* z) {
  returncode_t err;

  result.fired = false;

  err = libtock_ninedof_read_gyroscope(ninedof_cb);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *x = result.x;
  *y = result.y;
  *z = result.z;

  return RETURNCODE_SUCCESS;
}
