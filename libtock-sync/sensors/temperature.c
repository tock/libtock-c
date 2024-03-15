#include <sensors/temperature.h>

#include "temperature.h"


struct data {
  bool fired;
  int temp;
  returncode_t result;
};

static struct data result = { .fired = false };


static void temp_cb(returncode_t ret, int temperature) {
  result.temp   = temperature;
  result.fired  = true;
  result.result = ret;
}


returncode_t libtocksync_temperature_read(int* temperature) {
  returncode_t err;
  result.fired = false;

  err = libtock_temperature_read(temp_cb);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  if (result.result != RETURNCODE_SUCCESS) return result.result;

  *temperature = result.temp;

  return RETURNCODE_SUCCESS;
}
