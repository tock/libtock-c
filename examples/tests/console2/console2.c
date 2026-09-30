#include <stdbool.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/defer.h>

#include "console2.h"

#define DRIVER_NUM_CONSOLE2 (DRIVER_NUM_CONSOLE | 0x01000000)

#define CONSOLE2_COMMAND_WRITE 1
#define CONSOLE2_ALLOW_WRITE 1
#define CONSOLE2_SUBSCRIBE_WRITE 1

static returncode_t console2_set_read_allow(const uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_CONSOLE2, CONSOLE2_ALLOW_WRITE, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

static returncode_t console2_command_write(int length) {
  syscall_return_t cval = command(DRIVER_NUM_CONSOLE2, CONSOLE2_COMMAND_WRITE, length, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

static returncode_t console2_yield_wait_for_write(uint32_t* bytes_written) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_CONSOLE2, CONSOLE2_SUBSCRIBE_WRITE);

  *bytes_written = ret.data1;

  return tock_status_to_returncode(ret.data0);
}


bool console2_driver_exists(void) {
  return driver_exists(DRIVER_NUM_CONSOLE2);
}

returncode_t console2_sync_write(const uint8_t* buffer, uint32_t length, uint32_t* written) {
  int err;

  err = console2_set_read_allow(buffer, length);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { console2_set_read_allow(NULL, 0);
  }

  err = console2_command_write((int) length);
  if (err != RETURNCODE_SUCCESS) return err;

  err = console2_yield_wait_for_write(written);
  return err;
}
