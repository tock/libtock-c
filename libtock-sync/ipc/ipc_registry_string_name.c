#include <string.h>

#include <libtock/defer.h>

#include "ipc_registry_string_name.h"

#include "syscalls/ipc_registry_string_name_syscalls.h"

#define STRING_BUF_LEN 20
static uint8_t string_buf[STRING_BUF_LEN] = {0};

bool libtocksync_ipc_registry_string_name_exists(void) {
  return libtock_ipc_registry_string_name_driver_exists();
}

returncode_t libtocksync_ipc_registry_string_name_register_service(uint8_t* string_name, size_t string_name_len) {
  // Copy data to local buffer if possible, which is fixed in size and zero-padded
  if (string_name_len > STRING_BUF_LEN) {
    return RETURNCODE_ESIZE;
  }
  memset(string_buf, 0, STRING_BUF_LEN);
  for (size_t i = 0; i < STRING_BUF_LEN && i < string_name_len; i++) {
    string_buf[i] = string_name[i];
  }

  returncode_t err = RETURNCODE_SUCCESS;
  err = libtock_ipc_registry_string_name_set_allow_name(string_buf, STRING_BUF_LEN);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_ipc_registry_string_name_set_allow_name(NULL, 0);
  }

  err = libtock_ipc_registry_string_name_command_register();
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_ipc_registry_string_name_yield_wait_for_registration_complete();
  return err;
}

returncode_t libtocksync_ipc_registry_string_name_discover_service(uint8_t* string_name, size_t string_name_len,
                                                                   uint64_t* ipc_id) {
  // Copy data to local buffer if possible, which is fixed in size and zero-padded
  if (string_name_len > STRING_BUF_LEN) {
    return RETURNCODE_ESIZE;
  }
  memset(string_buf, 0, STRING_BUF_LEN);
  for (size_t i = 0; i < STRING_BUF_LEN && i < string_name_len; i++) {
    string_buf[i] = string_name[i];
  }

  returncode_t err = RETURNCODE_SUCCESS;
  err = libtock_ipc_registry_string_name_set_allow_name(string_buf, STRING_BUF_LEN);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_ipc_registry_string_name_set_allow_name(NULL, 0);
  }

  err = libtock_ipc_registry_string_name_command_discover();
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_ipc_registry_string_name_yield_wait_for_discovery_complete(ipc_id);
  return err;
}
