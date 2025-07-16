#include <libtock/storage/syscalls/app_state_syscalls.h>

#include "app_state.h"

struct app_state_data {
  bool fired;
  returncode_t ret;
};

static struct app_state_data result = {.fired = false};

static void app_state_cb(returncode_t ret) {
  result.fired = true;
  result.ret   = ret;
}

returncode_t libtocksync_app_state_save(void) {
  returncode_t err;

  result.fired = false;

  err = libtock_app_state_save(app_state_cb);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return result.ret;
}
