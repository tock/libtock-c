#pragma once

#include "../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for clients, response received.
//
// - `arg1` (`returncode_t`): Status for response
// - `arg2` (`size_t`): Length of response
typedef void (*libtock_ipc_relay_request_client_response_received)(returncode_t, size_t);

// Function signature for servers, request waiting.
//
// - `arg1` (`uint64_t`): IPC identifier for waiting request
typedef void (*libtock_ipc_relay_request_server_request_waiting)(uint64_t);


// Check if the driver exists.
//
// ## Return value
//
// True if the driver exists.
bool libtock_ipc_relay_request_exists(void);

// For clients, send a request to a service.
//
// If successful, the callback will be received when the transaction is
// completed.
//
// ## Arguments
//
// `cb`: Callback function triggered when the transaction is completed.
// `ipc_id`: IPC identifier of service to send request to.
// `request_buf`: Buffer of uint8_t values to send to the service.
// `request_len`: Length of request buffer.
// `response_buf`: Buffer of uint8_t values to hold response from service.
// `response_len`: Length of response buffer.
//
// ## Return value
//
// A returncode indicating whether the request has been started successfully.
returncode_t libtock_ipc_relay_request_client_send_request(
  libtock_ipc_relay_request_client_response_received cb,
  uint64_t ipc_id,
  uint8_t* request_buf, size_t request_len,
  uint8_t* response_buf, size_t response_len);

// For clients, cancel an outstanding request.
//
// ## Return value
//
// A returncode indicating whether the request has been canceled successfully.
returncode_t libtock_ipc_relay_request_client_cancel_request(void);

// For servers, enable requests and callbacks when new requests are waiting for
// this server.
//
// If successful, the callback will be triggered when any future request becomes
// available for this server.
//
// ## Arguments
//
// `cb`: Callback function triggered when a new request is ready
//
// ## Return value
//
// A returncode indicating whether the callback was installed successfully.
returncode_t libtock_ipc_relay_request_server_enable_requests_with_callback(
  libtock_ipc_relay_request_server_request_waiting cb);

// For servers, enable requests without registering for callbacks.
//
// Without callbacks, the server will need to periodically get_next_request to
// see if any requests are waiting for it.
//
// ## Return value
//
// A returncode indicating whether the callback was installed successfully.
returncode_t libtock_ipc_relay_request_server_enable_requests_without_callback(void);

// For servers, disable request waiting callbacks.
//
// ## Return value
//
// A returncode indicating whether the callback was removed successfully.
returncode_t libtock_ipc_relay_request_server_disable_requests(void);

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
returncode_t libtock_ipc_relay_request_server_get_next_request(
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
//      retrieved. If no request was retrieved, `request_len` will be set to zero.
returncode_t libtock_ipc_relay_request_server_get_next_request_from(
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
returncode_t libtock_ipc_relay_request_server_send_response(
  uint8_t* response_buf, size_t response_len);


#ifdef __cplusplus
}
#endif
