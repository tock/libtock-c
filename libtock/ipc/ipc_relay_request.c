#include "ipc_relay_request.h"

#include "syscalls/ipc_relay_request_syscalls.h"

// callback for client, response received
static void client_response_received(int statuscode, int len,
                                     __attribute__ ((unused)) int unused,
                                     void* opaque) {
  // Cleanup before performing callback
  libtock_ipc_relay_request_set_allow_read_buffer(NULL, 0);
  libtock_ipc_relay_request_set_allow_write_buffer(NULL, 0);
  libtock_ipc_relay_request_set_upcall_client_response_received(NULL, NULL);

  libtock_ipc_relay_request_client_response_received cb = (libtock_ipc_relay_request_client_response_received)opaque;
  cb(tock_status_to_returncode(statuscode), (size_t)((uint32_t)len));
}

// callback for server, request waiting
static void server_request_waiting(int lower_val, int upper_val,
                                   __attribute__ ((unused)) int unused,
                                   void* opaque) {
  libtock_ipc_relay_request_server_request_waiting cb = (libtock_ipc_relay_request_server_request_waiting)opaque;
  uint32_t lower  = (uint32_t)lower_val;
  uint32_t upper  = (uint32_t)upper_val;
  uint64_t ipc_id = (((uint64_t)upper) << 32) | ((uint64_t)lower);
  cb(ipc_id);
}

bool libtock_ipc_relay_request_exists(void) {
  return libtock_ipc_relay_request_driver_exists();
}

returncode_t libtock_ipc_relay_request_client_send_request(
  libtock_ipc_relay_request_client_response_received cb,
  uint64_t ipc_id,
  uint8_t* request_buf, size_t request_len,
  uint8_t* response_buf, size_t response_len) {
  returncode_t err = RETURNCODE_SUCCESS;

  err = libtock_ipc_relay_request_set_allow_read_buffer(request_buf, request_len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_ipc_relay_request_set_allow_write_buffer(response_buf, response_len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_ipc_relay_request_set_upcall_client_response_received(client_response_received, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_ipc_relay_request_command_client_send_request(ipc_id);
  return err;
}

returncode_t libtock_ipc_relay_request_client_cancel_request(void) {
  return libtock_ipc_relay_request_command_client_cancel_request();
}

returncode_t libtock_ipc_relay_request_server_enable_requests_with_callback(
  libtock_ipc_relay_request_server_request_waiting cb) {
  returncode_t err = RETURNCODE_SUCCESS;

  err = libtock_ipc_relay_request_set_upcall_server_request_waiting(server_request_waiting, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_ipc_relay_request_command_server_enable_requests();
  return err;
}

returncode_t libtock_ipc_relay_request_server_enable_requests_without_callback(void) {
  return libtock_ipc_relay_request_command_server_enable_requests();
}

returncode_t libtock_ipc_relay_request_server_disable_requests(void) { \
  returncode_t err = RETURNCODE_SUCCESS;

  err = libtock_ipc_relay_request_command_server_disable_requests();
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_ipc_relay_request_set_upcall_server_request_waiting(NULL, NULL);
  return err;
}

returncode_t libtock_ipc_relay_request_server_get_next_request(
  uint64_t* ipc_id,
  uint8_t* request_buf, size_t* request_len) {
  returncode_t err = RETURNCODE_SUCCESS;

  err = libtock_ipc_relay_request_set_allow_write_buffer(request_buf, *request_len);
  if (err != RETURNCODE_SUCCESS) return err;

  size_t max_request_len = *request_len;
  err = libtock_ipc_relay_request_command_server_get_next_request((uint32_t*)request_len, ipc_id);
  if (err == RETURNCODE_ESIZE) {
    // Request buffer size was too small, up to maximum was copied
    *request_len = max_request_len;
  } else if (err != RETURNCODE_SUCCESS) {
    // Error condition, no data is in the buffer
    *request_len = 0;
  }

  // Cleanup
  libtock_ipc_relay_request_set_allow_write_buffer(NULL, 0);

  return err;
}

returncode_t libtock_ipc_relay_request_server_get_next_request_from(
  uint64_t ipc_id,
  uint8_t* request_buf, size_t* request_len) {
  returncode_t err = RETURNCODE_SUCCESS;

  err = libtock_ipc_relay_request_set_allow_write_buffer(request_buf, *request_len);
  if (err != RETURNCODE_SUCCESS) return err;

  uint64_t ipc_id_copy   = ipc_id;
  size_t max_request_len = *request_len;
  err = libtock_ipc_relay_request_command_server_get_next_request_from((uint32_t*)request_len, &ipc_id_copy);
  if (err == RETURNCODE_ESIZE) {
    // Request buffer size was too small, up to maximum was copied
    *request_len = max_request_len;
  } else if (err != RETURNCODE_SUCCESS) {
    // Error condition, no data is in the buffer
    *request_len = 0;
  }

  // Cleanup
  libtock_ipc_relay_request_set_allow_write_buffer(NULL, 0);

  return err;
}

returncode_t libtock_ipc_relay_request_server_send_response(
  uint8_t* response_buf, size_t response_len) {
  returncode_t err = RETURNCODE_SUCCESS;

  err = libtock_ipc_relay_request_set_allow_read_buffer(response_buf, response_len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_ipc_relay_request_command_server_send_response();

  // Cleanup
  libtock_ipc_relay_request_set_allow_read_buffer(NULL, 0);

  return err;
}
