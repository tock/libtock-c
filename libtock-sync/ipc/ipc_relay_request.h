#pragma once

#include <libtock/ipc/ipc_relay_request.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Determine if the IPC Registry String Name capsule exists.
//
// ## Return Value
//
// True if the capsule exists.
bool libtocksync_ipc_relay_request_exists(void);

// For clients, send a request to a service.
//
// Warning: if the server does not respond, this function will never return.
// While requests can be canceled, this synchronous interface has no ability to
// do so. See <libtock-sync/services/ipc_relay_request_timeout.h> for an
// alternative synchronous option with a timeout that is likely preferable.
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
//
// ## Return value
//
// A returncode indicating whether the request was sent and response
// successfully received.
returncode_t libtocksync_ipc_relay_request_client_send_request(
  uint64_t ipc_id,
  uint8_t* request_buf, size_t request_len,
  uint8_t* response_buf, size_t* response_len);

// For servers, enable requests from clients.
//
// ## Return value
//
// A returncode indicating whether requests have been enabled.
returncode_t libtocksync_ipc_relay_request_server_enable_requests(void);

// For servers, disable requests from clients.
//
// Note: this will send an error to any outstanding clients.
//
// ## Return value
//
// A returncode indicating whether requests have been canceled.
returncode_t libtocksync_ipc_relay_request_server_disable_requests(void);

// For servers, get the next waiting request (if any exist).
//
// A request can only be retrieved if no outstanding request is in progress.
// Once retrieved, the request will remain in progress until a response is sent.
//
// ## Arguments
//
// `ipc_id`: Pointer to location to hold the the IPC identifier for the found request.
// `request_buf`: Buffer of uint8_t values to hold request from client.
// `request_len`: Length of request buffer. This will be overwritten as the
//      length of the message stored in the request buffer (<= original maximum length)
//
// ## Return value
//
// A returncode indicating whether a request existed and was successfully
//      retrieved. If no request was retrieved, `ipc_id` is invalid and
//      `request_len` will be set to zero.
returncode_t libtocksync_ipc_relay_request_server_get_next_request(
  uint64_t* ipc_id,
  uint8_t* request_buf, size_t* request_len);

// For servers, get the next waiting request from a specified client (if it exists).
//
// A request can only be retrieved if no outstanding request is in progress.
// Once retrieved, the request will remain in progress until a response is sent.
//
// ## Arguments
//
// `ipc_id`: The IPC identifier of the client to get a request from.
// `request_buf`: Buffer of uint8_t values to hold request from client.
// `request_len`: Pointer to length of request buffer. This will be overwritten as the
//      length of the message stored in the request buffer (<= original maximum length)
//
// ## Return value
//
// A returncode indicating whether a request existed and was successfully
//      retrieved. If no request was retrieved, `ipc_id` is invalid and
//      `request_len` will be set to zero.
returncode_t libtocksync_ipc_relay_request_server_get_next_request_from(
  uint64_t ipc_id,
  uint8_t* request_buf, size_t* request_len);

// For servers, send a response to the client you got a request from.
//
// The request MUST first be retrieved with one of the `get_next_request`
// functions. The response will go to the client corresponding to that request.
//
// ## Arguments
//
// `response_buf`: Buffer of uint8_t values to send to the client.
// `response_len`: Length of response buffer.
//
// ## Return value
//
// A returncode indicating if the response was successfully sent.
returncode_t libtocksync_ipc_relay_request_server_send_response(
  uint8_t* response_buf, size_t response_len);


#ifdef __cplusplus
}
#endif
