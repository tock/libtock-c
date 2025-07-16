#include <libtock/sensors/syscalls/pressure_syscalls.h>

#include "pressure.h"

struct pressure_data {
  bool fired;
  int pressure;
  returncode_t ret;
};

static struct pressure_data result = { .fired = false };

static void pressure_cb(returncode_t ret, int pressure) {
  result.pressure = pressure;
  result.fired    = true;
  result.ret      = ret;
}

bool libtocksync_pressure_exists(void) {
  return libtock_pressure_driver_exists();
}

returncode_t libtocksync_pressure_read(int* pressure) {
  returncode_t err;

  result.fired = false;

  err = libtock_pressure_read(pressure_cb);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *pressure = result.pressure;

  return RETURNCODE_SUCCESS;
}
