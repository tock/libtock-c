#include "ipc_synchronous_mailbox_syscalls.h"

bool libtock_ipc_synchronous_mailbox_driver_exists(void) {
    return driver_exists(DRIVER_NUM_IPC_SYNCHRONOUS_MAXILBOX);
}

returncode_t libtock_ipc_synchronous_mailbox_command_client_send_request(uint64_t ipc_id) {
    uint32_t lower = (uint32_t)ipc_id;
    uint32_t upper = (uint32_t)(ipc_id >> 32);
    syscall_return_t cval = command(DRIVER_NUM_IPC_SYNCHRONOUS_MAXILBOX, 1, lower, upper);
    return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_ipc_synchronous_mailbox_command_client_cancel_request(void) {
    syscall_return_t cval = command(DRIVER_NUM_IPC_SYNCHRONOUS_MAXILBOX, 2, 0, 0);
    return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_ipc_synchronous_mailbox_command_server_get_next_request(uint32_t* len, uint64_t* ipc_id) {
    syscall_return_t cval = command(DRIVER_NUM_IPC_SYNCHRONOUS_MAXILBOX, 3, 0, 0);

    // We have different success and failure variants
    // Success with u32 and u64
    // Failure with u64
    if (cval.type == TOCK_SYSCALL_SUCCESS_U32_U64) {
        *len = cval.data[0];
        uint32_t lsb = cval.data[1];
        uint32_t msb = cval.data[2];
        *ipc_id = (((uint64_t)msb) << 32) | ((uint64_t)lsb);
        return RETURNCODE_SUCCESS;
    } else if (cval.type == TOCK_SYSCALL_FAILURE_U64) {
        // In this error case, len is either max or irrelevant depending on the returncode
        uint32_t lsb = cval.data[1];
        uint32_t msb = cval.data[2];
        *ipc_id = (((uint64_t)msb) << 32) | ((uint64_t)lsb);
        return tock_status_to_returncode(cval.data[0]);
    } else {
        // The remaining SyscallReturn variants must never happen if using this
        // function. We return `EBADRVAL` to signal an unexpected return variant.
        return RETURNCODE_EBADRVAL;
    }
}

returncode_t libtock_ipc_synchronous_mailbox_command_server_get_next_request_from(uint32_t* len, uint64_t* ipc_id) {
    uint32_t lower = (uint32_t)(*ipc_id);
    uint32_t upper = (uint32_t)((*ipc_id) >> 32);
    syscall_return_t cval = command(DRIVER_NUM_IPC_SYNCHRONOUS_MAXILBOX, 4, lower, upper);

    // We have different success and failure variants
    // Success with u32 and u64
    // Failure with u64
    if (cval.type == TOCK_SYSCALL_SUCCESS_U32_U64) {
        *len = cval.data[0];
        uint32_t lsb = cval.data[1];
        uint32_t msb = cval.data[2];
        *ipc_id = (((uint64_t)msb) << 32) | ((uint64_t)lsb);
        return RETURNCODE_SUCCESS;
    } else if (cval.type == TOCK_SYSCALL_FAILURE_U64) {
        // In this error case, len is either max or irrelevant depending on the returncode
        uint32_t lsb = cval.data[1];
        uint32_t msb = cval.data[2];
        *ipc_id = (((uint64_t)msb) << 32) | ((uint64_t)lsb);
        return tock_status_to_returncode(cval.data[0]);
    } else {
        // The remaining SyscallReturn variants must never happen if using this
        // function. We return `EBADRVAL` to signal an unexpected return variant.
        return RETURNCODE_EBADRVAL;
    }
}

returncode_t libtock_ipc_synchronous_mailbox_command_server_send_response(void) {
    syscall_return_t cval = command(DRIVER_NUM_IPC_SYNCHRONOUS_MAXILBOX, 5, 0, 0);
    return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_ipc_synchronous_mailbox_set_upcall_client_response_received(subscribe_upcall callback, void* opaque) {
    subscribe_return_t sval = subscribe(DRIVER_NUM_IPC_SYNCHRONOUS_MAXILBOX, 0, callback, opaque);
    return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_ipc_synchronous_mailbox_set_upcall_server_request_waiting(subscribe_upcall callback, void* opaque) {
    subscribe_return_t sval = subscribe(DRIVER_NUM_IPC_SYNCHRONOUS_MAXILBOX, 1, callback, opaque);
    return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_ipc_synchronous_mailbox_set_allow_read_buffer(void* data, size_t len) {
    allow_ro_return_t aval = allow_readonly(DRIVER_NUM_IPC_SYNCHRONOUS_MAXILBOX, 0, data, len);
    return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_ipc_synchronous_mailbox_set_allow_write_buffer(void* data, size_t len) {
    allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_IPC_SYNCHRONOUS_MAXILBOX, 0, data, len);
    return tock_allow_rw_return_to_returncode(aval);
}
