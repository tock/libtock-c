#include <libtock/defer.h>
#include <libtock/storage/syscalls/sdcard_syscalls.h>

#include "sdcard.h"

#include "syscalls/sdcard_syscalls.h"

bool libtocksync_sdcard_exists(void) {
  return libtock_sdcard_driver_exists();
}

returncode_t libtocksync_sdcard_initialize(uint32_t* block_size, uint32_t* size_in_kB) {
  returncode_t ret;

  ret = libtock_sdcard_command_initialize();
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_sdcard_yield_wait_for_initialize(block_size, size_in_kB);
  return ret;
}

returncode_t libtocksync_sdcard_read_block(uint32_t sector, uint8_t* buffer, uint32_t len) {
  returncode_t ret;

  ret = libtock_sdcard_set_readwrite_allow_read_buffer(buffer, len);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_sdcard_set_readwrite_allow_read_buffer(NULL, 0);
  }

  ret = libtock_sdcard_command_read_block(sector);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_sdcard_yield_wait_for_operation();
  return ret;
}

returncode_t libtocksync_sdcard_write_block(uint32_t sector, uint8_t* buffer, uint32_t len) {
  returncode_t ret;

  ret = libtock_sdcard_set_readonly_allow_write_buffer(buffer, len);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_sdcard_set_readonly_allow_write_buffer(NULL, 0);
  }

  ret = libtock_sdcard_command_write_block(sector);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_sdcard_yield_wait_for_operation();
  return ret;
}
