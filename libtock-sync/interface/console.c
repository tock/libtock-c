#include "console.h"
#include "syscalls/console_syscalls.h"

returncode_t libtocksync_console_write(const uint8_t* buffer, uint32_t length, uint32_t* written) {
  returncode_t ret;

  ret = libtock_console_set_read_allow(buffer, length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_console_command_write(length);
  if (ret != RETURNCODE_SUCCESS) goto exit;

  ret = libtocksync_console_yield_wait_for_write(written);

exit:
  libtock_console_set_read_allow(NULL, 0);

  return ret;
}

returncode_t libtocksync_console_read(uint8_t* buffer, uint32_t length, uint32_t* read) {
  returncode_t ret;

  ret = libtock_console_set_readwrite_allow(buffer, length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_console_command_read(length);
  if (ret != RETURNCODE_SUCCESS) goto exit;

  ret = libtocksync_console_yield_wait_for_write(read);

exit:
  libtock_console_set_readwrite_allow(NULL, 0);

  return ret;
}
