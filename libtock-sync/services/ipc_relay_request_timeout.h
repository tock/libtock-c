#pragma once

#include <libtock/ipc/ipc_relay_request.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// For clients, send a request to a service.
//
// This uses the alarm driver to implement a timeout. If a timeout value is
// provided and the given number of milliseconds pass without receiving a
// response, the request will be canceled. If a timeout occurs, the function
// will return the value `RETURNCODE_ENOACK`.
//
// Be aware that this code internally calls `yield()` to wait for a callback
// that could be from either IPC or Alarm. This may also trigger other unrelated
// upcalls if any are outstanding.
//
// ## Arguments
//
// `ipc_id`: IPC identifier of service to send request to.
// `request_buf`: Buffer of uint8_t values to send to the service.
// `request_len`: Length of request buffer.
// `response_buf`: Buffer of uint8_t values to hold response from service.
// `response_len`: Pointer to length of response buffer. This will be
//      overwritten as the length of the message stored in the response buffer
//      (<= original maximum length)
// `timeout_ms`: Number of milliseconds to wait for a response before canceling
//      the request. Waits forever if the timeout is zero.
//
// ## Return value
//
// A returncode indicating whether the request was sent and response
// successfully received.
returncode_t libtocksync_ipc_relay_request_client_send_request_with_timeout(
  uint64_t ipc_id,
  uint8_t* request_buf, size_t request_len,
  uint8_t* response_buf, size_t* response_len,
  uint32_t timeout_ms);


#ifdef __cplusplus
}
#endif
