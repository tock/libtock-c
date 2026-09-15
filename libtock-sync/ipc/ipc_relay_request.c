#include <libtock/defer.h>

#include "ipc_relay_request.h"

#include "syscalls/ipc_relay_request_syscalls.h"

bool libtocksync_ipc_relay_request_exists(void) {
  return libtock_ipc_relay_request_driver_exists();
}

returncode_t libtocksync_ipc_relay_request_client_send_request(
  uint64_t ipc_id,
  uint8_t* request_buf, size_t request_len,
  uint8_t* response_buf, size_t* response_len) {
  returncode_t err = RETURNCODE_SUCCESS;

  err = libtock_ipc_relay_request_set_allow_read_buffer(request_buf, request_len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_ipc_relay_request_set_allow_read_buffer(NULL, 0);
  }

  err = libtock_ipc_relay_request_set_allow_write_buffer(response_buf, *response_len);
  if (err != RETURNCODE_SUCCESS) return err;
  defer { libtock_ipc_relay_request_set_allow_write_buffer(NULL, 0);
  }

  err = libtock_ipc_relay_request_command_client_send_request(ipc_id);
  if (err != RETURNCODE_SUCCESS) return err;

  // we do a yield-waitfor here, so this code will hang forever if the server
  // never bothers to respond
  err = libtocksync_ipc_relay_request_yield_wait_for_client_response_received(response_len);
  if (err != RETURNCODE_SUCCESS) return err;
  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_ipc_relay_request_server_enable_requests(void) {
  // Libtock function is already synchronous
  return libtock_ipc_relay_request_server_enable_requests_without_callback();
}

returncode_t libtocksync_ipc_relay_request_server_disable_requests(void) {
  // Libtock function is already synchronous
  return libtock_ipc_relay_request_server_disable_requests();
}

returncode_t libtocksync_ipc_relay_request_server_get_next_request(
  uint64_t* ipc_id,
  uint8_t* request_buf, size_t* request_len) {
  // Libtock function is already synchronous
  return libtock_ipc_relay_request_server_get_next_request(ipc_id, request_buf, request_len);
}

returncode_t libtocksync_ipc_relay_request_server_get_next_request_from(
  uint64_t ipc_id,
  uint8_t* request_buf, size_t* request_len) {
  // Libtock function is already synchronous
  return libtock_ipc_relay_request_server_get_next_request_from(ipc_id, request_buf, request_len);
}

returncode_t libtocksync_ipc_relay_request_server_send_response(
  uint8_t* response_buf, size_t response_len) {
  // Libtock function is already synchronous
  return libtock_ipc_relay_request_server_send_response(response_buf, response_len);
}
