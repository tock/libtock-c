#include "moisture.h"

typedef struct {
  int moisture;
  returncode_t ret;
  bool fired;
} moisture_result_t;

static moisture_result_t result = {.fired = false};

// callback for synchronous reads
static void moisture_callback(returncode_t ret, int moisture) {
  result.moisture = moisture;
  result.ret      = ret;
  result.fired    = true;
}

returncode_t libtocksync_moisture_read(int* moisture) {
  returncode_t err;

  result.fired = false;

  err = libtock_moisture_read(moisture_callback);
  if (err != RETURNCODE_SUCCESS) return err;

  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *moisture = result.moisture;

  return RETURNCODE_SUCCESS;
}
