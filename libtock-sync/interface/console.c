#include <libtock/interface/syscalls/console_syscalls.h>

#include "console.h"

struct console_result {
  bool fired;
  int length;
  returncode_t result;
};

static struct console_result result = { .fired = false };

static void generic_cb(returncode_t ret, uint32_t length) {
  result.length = length;
  result.fired  = true;
  result.result = ret;
}

bool libtocksync_console_exists(void) {
  return libtock_console_driver_exists();
}

returncode_t libtocksync_console_write(const uint8_t* buffer, uint32_t length, int* written) {
  int err;
  result.fired = false;

  err = libtock_console_write(buffer, length, &generic_cb);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  if (result.result != RETURNCODE_SUCCESS) return result.result;

  *written = result.length;
  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_console_read(uint8_t* buffer, uint32_t length, int* read) {
  int err;
  result.fired = false;

  err = libtock_console_read(buffer, length, &generic_cb);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  if (result.result != RETURNCODE_SUCCESS) return result.result;

  *read = result.length;
  return RETURNCODE_SUCCESS;
}
