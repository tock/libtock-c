#include <libtock/sensors/syscalls/rainfall_syscalls.h>

#include "rainfall.h"

typedef struct {
  uint32_t rainfall;
  returncode_t ret;
  bool fired;
} rainfall_result_t;

static rainfall_result_t result = {.fired = false};

// callback for synchronous reads
static void rainfall_callback(returncode_t ret, uint32_t rainfall) {
  result.rainfall = rainfall;
  result.ret      = ret;
  result.fired    = true;
}

bool libtocksync_rainfall_exists(void) {
  return libtock_rainfall_driver_exists();
}

returncode_t libtocksync_rainfall_read(uint32_t* rainfall, int hours) {
  returncode_t err;

  result.fired = false;

  err = libtock_rainfall_read(rainfall_callback, hours);
  if (err != RETURNCODE_SUCCESS) return err;

  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *rainfall = result.rainfall;

  return RETURNCODE_SUCCESS;
}
