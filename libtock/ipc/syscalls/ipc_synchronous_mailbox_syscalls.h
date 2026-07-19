#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_IPC_SYNCHRONOUS_MAXILBOX 0xA0010

bool libtock_ipc_synchronous_mailbox_driver_exists(void);

returncode_t libtock_ipc_synchronous_mailbox_command_client_send_request(uint64_t ipc_id);

returncode_t libtock_ipc_synchronous_mailbox_command_client_cancel_request(void);

returncode_t libtock_ipc_synchronous_mailbox_command_server_get_next_request(uint32_t* len, uint64_t* ipc_id);

returncode_t libtock_ipc_synchronous_mailbox_command_server_get_next_request_from(uint32_t* len, uint64_t* ipc_id);

returncode_t libtock_ipc_synchronous_mailbox_command_server_send_response(void);

returncode_t libtock_ipc_synchronous_mailbox_set_upcall_client_response_received(subscribe_upcall callback, void* opaque);

returncode_t libtock_ipc_synchronous_mailbox_set_upcall_server_request_waiting(subscribe_upcall callback, void* opaque);

returncode_t libtock_ipc_synchronous_mailbox_set_allow_read_buffer(void* data, size_t len);

returncode_t libtock_ipc_synchronous_mailbox_set_allow_write_buffer(void* data, size_t len);


#ifdef __cplusplus
}
#endif
