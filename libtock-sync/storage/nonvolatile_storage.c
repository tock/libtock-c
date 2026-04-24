#include <libtock/defer.h>
#include <libtock/storage/syscalls/nonvolatile_storage_syscalls.h>

#include "nonvolatile_storage.h"

#include "syscalls/nonvolatile_storage_syscalls.h"

bool libtocksync_nonvolatile_storage_exists(void) {
  return libtock_nonvolatile_storage_driver_exists();
}

returncode_t libtocksync_nonvolatile_storage_write(uint32_t offset, uint32_t length, uint8_t* buffer,
                                                   uint32_t buffer_length, int* length_written) {
  returncode_t ret;

  ret = libtock_nonvolatile_storage_set_allow_readonly_write_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_nonvolatile_storage_set_allow_readonly_write_buffer(NULL, 0);
  }

  ret = libtock_nonvolatile_storage_command_write(offset, length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_nonvolatile_storage_yield_wait_for_write(length_written);
  return ret;
}

returncode_t libtocksync_nonvolatile_storage_read(uint32_t offset, uint32_t length, uint8_t* buffer,
                                                  uint32_t buffer_length, int* length_read) {
  returncode_t ret;

  ret = libtock_nonvolatile_storage_set_allow_readwrite_read_buffer(buffer, buffer_length);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_nonvolatile_storage_set_allow_readwrite_read_buffer(NULL, 0);
  }

  ret = libtock_nonvolatile_storage_command_read(offset, length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_nonvolatile_storage_yield_wait_for_read(length_read);
  return ret;
}
