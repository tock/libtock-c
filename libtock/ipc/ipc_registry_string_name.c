#include <string.h>

#include "ipc_registry_string_name.h"

#include "syscalls/ipc_registry_string_name_syscalls.h"

#define STRING_BUF_LEN 20
static uint8_t string_buf[STRING_BUF_LEN] = {0};

// callback for registration complete
static void registration_complete_upcall(int statuscode,
                                         __attribute__ ((unused)) int unused1,
                                         __attribute__ ((unused)) int unused2, void* opaque) {
  // Cleanup before performing callback
  libtock_ipc_registry_string_name_set_allow_name(NULL, 0);
  libtock_ipc_registry_string_name_set_upcall_registration_complete(NULL, NULL);

  libtock_ipc_registry_string_name_registration_complete_callback cb =
    (libtock_ipc_registry_string_name_registration_complete_callback)opaque;
  cb(tock_status_to_returncode(statuscode));
}

// callback for discovery complete
static void discovery_complete_upcall(int statuscode, int lower_val, int upper_val, void* opaque) {
  // Cleanup before performing callback
  libtock_ipc_registry_string_name_set_allow_name(NULL, 0);
  libtock_ipc_registry_string_name_set_upcall_discovery_complete(NULL, NULL);

  libtock_ipc_registry_string_name_discovery_complete_callback cb =
    (libtock_ipc_registry_string_name_discovery_complete_callback)opaque;
  uint32_t lower  = (uint32_t)lower_val;
  uint32_t upper  = (uint32_t)upper_val;
  uint64_t ipc_id = (((uint64_t)upper) << 32) | ((uint64_t)lower);
  cb(tock_status_to_returncode(statuscode), ipc_id);
}

bool libtock_ipc_registry_string_name_exists(void) {
  return libtock_ipc_registry_string_name_driver_exists();
}

returncode_t libtock_ipc_registry_string_name_register_service(
  libtock_ipc_registry_string_name_registration_complete_callback cb,
  uint8_t*                                                        string_name,
  size_t                                                          string_name_len) {
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

  err = libtock_ipc_registry_string_name_set_upcall_registration_complete(registration_complete_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_ipc_registry_string_name_command_register();
  return err;
}

returncode_t libtock_ipc_registry_string_name_discover_service(
  libtock_ipc_registry_string_name_discovery_complete_callback cb,
  uint8_t*                                                     string_name,
  size_t                                                       string_name_len) {

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

  err = libtock_ipc_registry_string_name_set_upcall_discovery_complete(discovery_complete_upcall, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_ipc_registry_string_name_command_discover();
  return err;
}
