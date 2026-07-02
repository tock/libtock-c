#include <libtock/defer.h>
#include <libtock/storage/syscalls/isolated_nonvolatile_storage_syscalls.h>

#include "isolated_nonvolatile_storage.h"

#include "syscalls/isolated_nonvolatile_storage_syscalls.h"

bool libtocksync_isolated_nonvolatile_storage_exists(void) {
  return libtock_isolated_nonvolatile_storage_driver_exists();
}

returncode_t libtocksync_isolated_nonvolatile_storage_get_number_bytes(uint64_t* number_bytes) {
  returncode_t ret;

  ret = libtock_isolated_nonvolatile_storage_command_get_number_bytes();
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_isolated_nonvolatile_storage_yield_wait_for_get_number_bytes(number_bytes);
  return ret;
}

returncode_t libtocksync_isolated_nonvolatile_storage_write(uint64_t offset, uint8_t* buffer,
                                                            uint32_t buffer_length) {
  returncode_t ret;

  ret = libtock_isolated_nonvolatile_storage_set_allow_readonly_write_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_isolated_nonvolatile_storage_set_allow_readonly_write_buffer(NULL, 0);
  }

  ret = libtock_isolated_nonvolatile_storage_command_write(offset);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_isolated_nonvolatile_storage_yield_wait_for_write();
  return ret;
}

returncode_t libtocksync_isolated_nonvolatile_storage_read(uint64_t offset, uint8_t* buffer,
                                                           uint32_t buffer_length) {
  returncode_t ret;

  ret = libtock_isolated_nonvolatile_storage_set_allow_readwrite_read_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_isolated_nonvolatile_storage_set_allow_readwrite_read_buffer(NULL, 0);
  }

  ret = libtock_isolated_nonvolatile_storage_command_read(offset);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_isolated_nonvolatile_storage_yield_wait_for_read();
  return ret;
}
