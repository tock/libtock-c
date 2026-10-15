#include <stdio.h>
#include <string.h>

#include <libtock/defer.h>
#include <libtock/kernel/syscalls/app_loader_syscalls.h>

#include "app_loader.h"

#include "syscalls/app_loader_syscalls.h"

#define FLASH_BUFFER_SIZE 4096

bool libtocksync_app_loader_exists(void) {
  return libtock_app_loader_driver_exists();
}

returncode_t libtocksync_app_loader_setup(uint32_t app_length) {
  returncode_t ret;

  ret = libtock_app_loader_command_setup(app_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return libtocksync_app_loader_yield_wait_for_setup();
}

returncode_t libtocksync_app_loader_write(uint32_t flash_offset, uint32_t write_length, uint8_t* buffer) {
  returncode_t ret;
  static uint8_t write_buffer[FLASH_BUFFER_SIZE];

  if (write_length > FLASH_BUFFER_SIZE) {
    printf("[libtock] chunk length greater than flash buffer size\n");
    return RETURNCODE_FAIL;
  }

  ret = libtock_app_loader_write_buffer(buffer, FLASH_BUFFER_SIZE);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_app_loader_write_buffer(NULL, 0);
  }

  memcpy(write_buffer, buffer, write_length);
  if (write_length < FLASH_BUFFER_SIZE) {
    memset(write_buffer + write_length, 0, FLASH_BUFFER_SIZE - write_length);
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

returncode_t libtocksync_app_loader_unload(uint32_t short_id, uint32_t* app_handle) {
  returncode_t ret;

  ret = libtock_app_loader_command_unload(short_id);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return libtocksync_app_loader_yield_wait_for_unload(app_handle);
}

returncode_t libtocksync_app_loader_uninstall_with_app_handle(uint32_t app_handle) {
  returncode_t ret;

  ret = libtock_app_loader_command_uninstall_with_app_handle(app_handle);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return libtocksync_app_loader_yield_wait_for_uninstall_with_app_handle();
}
