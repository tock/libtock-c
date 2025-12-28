#include "ipc_registry_string_name_syscalls.h"

bool libtock_ipc_registry_string_name_driver_exists(void) {
  return driver_exists(DRIVER_NUM_IPC_REGISTRY_STRING_NAME);
}

returncode_t libtock_ipc_registry_string_name_set_upcall_registration_complete(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_IPC_REGISTRY_STRING_NAME, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_ipc_registry_string_name_set_upcall_discovery_complete(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_IPC_REGISTRY_STRING_NAME, 1, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_ipc_registry_string_name_set_upcall_new_registration(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_IPC_REGISTRY_STRING_NAME, 2, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_ipc_registry_string_name_set_allow_string_name(void* data, int len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_IPC_REGISTRY_STRING_NAME, 0, data, len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_ipc_registry_string_name_command_register(void) {
  syscall_return_t cval = command(DRIVER_NUM_IPC_REGISTRY_STRING_NAME, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_ipc_registry_string_name_command_discover(void) {
  syscall_return_t cval = command(DRIVER_NUM_IPC_REGISTRY_STRING_NAME, 2, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_ipc_registry_string_name_command_enable_registration_notifications(void) {
  syscall_return_t cval = command(DRIVER_NUM_IPC_REGISTRY_STRING_NAME, 3, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_ipc_registry_string_name_command_disable_registration_notifications(void) {
  syscall_return_t cval = command(DRIVER_NUM_IPC_REGISTRY_STRING_NAME, 4, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
