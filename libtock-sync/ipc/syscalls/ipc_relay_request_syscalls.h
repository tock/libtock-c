#pragma once

#include <libtock/ipc/syscalls/ipc_relay_request_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// For clients, wait for a response
returncode_t libtocksync_ipc_relay_request_yield_wait_for_client_response_received(size_t* response_len);

// For servers, wait for an incoming request
returncode_t libtocksync_ipc_relay_request_yield_wait_for_server_request_waiting(uint64_t* ipc_id);

#ifdef __cplusplus
}
#endif
