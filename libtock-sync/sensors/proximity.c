#include <libtock/sensors/syscalls/proximity_syscalls.h>

#include "proximity.h"

struct data {
  bool fired;
  uint8_t proximity;
  returncode_t ret;
};

static struct data result = {.fired = false};

static void proximity_cb(returncode_t ret, uint8_t proximity) {
  result.proximity = proximity;
  result.ret       = ret;
  result.fired     = true;
}

bool libtocksync_proximity_exists(void) {
  return libtock_proximity_driver_exists();
}

returncode_t libtocksync_proximity_read(uint8_t* proximity) {
  returncode_t err;
  result.fired = false;

  err = libtock_proximity_read(proximity_cb);
  if (err != RETURNCODE_SUCCESS) return err;

  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *proximity = result.proximity;

  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_proximity_read_on_interrupt(uint32_t lower_threshold, uint32_t higher_threshold,
                                                     uint8_t* proximity) {
  returncode_t err;
  result.fired = false;

  err = libtock_proximity_read_on_interrupt(lower_threshold, higher_threshold, proximity_cb);
  if (err != RETURNCODE_SUCCESS) return err;

  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *proximity = result.proximity;

  return RETURNCODE_SUCCESS;
}
