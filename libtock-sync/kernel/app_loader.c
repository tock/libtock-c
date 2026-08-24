#include <libtock/defer.h>
#include <libtock/kernel/syscalls/app_loader_syscalls.h>

#include "app_loader.h"

#include "syscalls/app_loader_syscalls.h"

bool libtocksync_app_loader_exists(void) {
  return libtock_app_loader_driver_exists();
}

returncode_t libtocksync_app_loader_setup(uint32_t app_length) {
  returncode_t ret;

  ret = libtock_app_loader_command_setup(app_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return libtocksync_app_loader_yield_wait_for_setup();
}

returncode_t libtocksync_app_loader_write(uint32_t flash_offset, uint32_t write_length, uint8_t* buffer,
                                          uint32_t buffer_len) {
  returncode_t ret;

  ret = libtock_app_loader_write_buffer(buffer, buffer_len);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_app_loader_write_buffer(NULL, 0);
  }

  ret = libtock_app_loader_command_write(flash_offset, write_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return libtocksync_app_loader_yield_wait_for_write();
}

returncode_t libtocksync_app_loader_finalize(void) {
  returncode_t ret;

  ret = libtock_app_loader_command_finalize();
  if (ret != RETURNCODE_SUCCESS) return ret;

  return libtocksync_app_loader_yield_wait_for_finalize();
}

returncode_t libtocksync_app_loader_load(void) {
  returncode_t ret;

  ret = libtock_app_loader_command_load();
  if (ret != RETURNCODE_SUCCESS) return ret;

  return libtocksync_app_loader_yield_wait_for_load();
}

returncode_t libtocksync_app_loader_abort(void) {
  returncode_t ret;

  ret = libtock_app_loader_command_abort();
  if (ret != RETURNCODE_SUCCESS) return ret;

  return libtocksync_app_loader_yield_wait_for_abort();
}
