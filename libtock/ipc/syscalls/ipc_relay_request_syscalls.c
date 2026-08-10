#include "ipc_relay_request_syscalls.h"

bool libtock_ipc_relay_request_driver_exists(void) {
  return driver_exists(DRIVER_NUM_IPC_RELAY_REQUEST);
}

returncode_t libtock_ipc_relay_request_command_client_send_request(uint64_t ipc_id) {
  uint32_t lower        = (uint32_t)ipc_id;
  uint32_t upper        = (uint32_t)(ipc_id >> 32);
  syscall_return_t cval = command(DRIVER_NUM_IPC_RELAY_REQUEST, 0x10, lower, upper);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_ipc_relay_request_command_client_cancel_request(void) {
  syscall_return_t cval = command(DRIVER_NUM_IPC_RELAY_REQUEST, 0x11, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_ipc_relay_request_command_server_enable_requests(void) {
  syscall_return_t cval = command(DRIVER_NUM_IPC_RELAY_REQUEST, 0x20, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_ipc_relay_request_command_server_disable_requests(void) {
  syscall_return_t cval = command(DRIVER_NUM_IPC_RELAY_REQUEST, 0x21, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_ipc_relay_request_command_server_get_next_request(uint32_t* len, uint64_t* ipc_id) {
  syscall_return_t cval = command(DRIVER_NUM_IPC_RELAY_REQUEST, 0x22, 0, 0);

  // We have two options here:
  // * success returns length and ipc_id
  // * failure returns just ipc_id, with length either max or irrelevant
  //   depending on the returncode, which should be handled by higher layers
  return tock_command_return_u32_u64_or_u64_to_returncode(cval, len, ipc_id);
}

returncode_t libtock_ipc_relay_request_command_server_get_next_request_from(uint32_t* len, uint64_t* ipc_id) {
  uint32_t lower        = (uint32_t)(*ipc_id);
  uint32_t upper        = (uint32_t)((*ipc_id) >> 32);
  syscall_return_t cval = command(DRIVER_NUM_IPC_RELAY_REQUEST, 0x23, lower, upper);

  // We have two options here:
  // * success returns length and ipc_id
  // * failure returns just ipc_id, with length either max or irrelevant
  //   depending on the returncode, which should be handled by higher layers
  return tock_command_return_u32_u64_or_u64_to_returncode(cval, len, ipc_id);
}

returncode_t libtock_ipc_relay_request_command_server_send_response(void) {
  syscall_return_t cval = command(DRIVER_NUM_IPC_RELAY_REQUEST, 0x24, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_ipc_relay_request_set_upcall_client_response_received(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_IPC_RELAY_REQUEST, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_ipc_relay_request_set_upcall_server_request_waiting(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_IPC_RELAY_REQUEST, 1, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_ipc_relay_request_set_allow_read_buffer(void* data, size_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_IPC_RELAY_REQUEST, 0, data, len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_ipc_relay_request_set_allow_write_buffer(void* data, size_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_IPC_RELAY_REQUEST, 0, data, len);
  return tock_allow_rw_return_to_returncode(aval);
}
