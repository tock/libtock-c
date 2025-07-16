#include <libtock/sensors/syscalls/ambient_light_syscalls.h>

#include "ambient_light.h"

typedef struct {
  int intensity;
  returncode_t ret;
  bool fired;
} ambient_light_data_t;

static ambient_light_data_t result = {.fired = false};

// callback for synchronous reads
static void ambient_light_callback(returncode_t ret, int intensity) {
  result.intensity = intensity;
  result.ret       = ret;
  result.fired     = true;
}

bool libtocksync_ambient_light_exists(void) {
  return libtock_ambient_light_driver_exists();
}

returncode_t libtocksync_ambient_light_read_intensity(int* lux_value) {
  returncode_t err;

  result.fired = false;

  err = libtock_ambient_light_read_intensity(ambient_light_callback);
  if (err != RETURNCODE_SUCCESS) return err;

  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *lux_value = result.intensity;

  return RETURNCODE_SUCCESS;
}
