#include "humidity.h"

typedef struct {
  int humidity;
  returncode_t ret;
  bool fired;
} humidity_result_t;

static humidity_result_t result = {.fired = false};

// callback for synchronous reads
static void humidity_callback(returncode_t ret, int humidity) {
  result.humidity = humidity;
  result.ret      = ret;
  result.fired    = true;
}

returncode_t libtocksync_humidity_read(int* humidity) {
  returncode_t err;

  result.fired = false;

  err = libtock_humidity_read(humidity_callback);
  if (err != RETURNCODE_SUCCESS) return err;

  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *humidity = result.humidity;

  return RETURNCODE_SUCCESS;
}
