#include <libtock/defer.h>
#include <libtock/interface/syscalls/console_syscalls.h>

#include "console.h"

#include "syscalls/console_syscalls.h"

bool libtocksync_console_exists(void) {
  return libtock_console_driver_exists();
}

returncode_t libtocksync_console_write(const uint8_t* buffer, uint32_t length, uint32_t* written) {
  int err;

  err = libtock_console_set_read_allow(buffer, length);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_console_set_read_allow(NULL, 0);
  }

  err = libtock_console_command_write((int) length);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_console_yield_wait_for_write(written);
  return err;
}

returncode_t libtocksync_console_read(uint8_t* buffer, uint32_t length, uint32_t* read) {
  int err;

  err = libtock_console_set_readwrite_allow(buffer, length);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_console_set_readwrite_allow(NULL, 0);
  }

  err = libtock_console_command_read((int) length);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_console_yield_wait_for_read(read);
  return err;
}
