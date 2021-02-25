#include "ipc.h"
#include "tock.h"

int ipc_discover(const char* pkg_name) {
  int len = strlen(pkg_name);
  allow_ro_return_t prev = allow_readonly(IPC_DRIVER_NUM, 0, pkg_name, len);
  syscall_return_t res   = command2(IPC_DRIVER_NUM, 1, 0, 0);
  prev = allow_readonly(IPC_DRIVER_NUM, 0, prev.ptr, prev.size);
  if (res.type != TOCK_SYSCALL_SUCCESS_U32) {
    return tock_error_to_rcode(res.data[0]);
  } else {
    return res.data[0];
  }
}

int ipc_register_service_callback(subscribe_upcall callback, void *ud) {
  subscribe_return_t sval = subscribe2(IPC_DRIVER_NUM, 0, callback, ud);
  if (sval.success) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(sval.error);
  }
}

int ipc_register_client_callback(int svc_id, subscribe_upcall callback, void *ud) {
  if (svc_id <= 0) {
    return TOCK_FAIL;
  }
  subscribe_return_t sval = subscribe2(IPC_DRIVER_NUM, svc_id, callback, ud);
  if (sval.success) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(sval.error);
  }
}

int ipc_notify_service(int pid) {
  syscall_return_t res = command2(IPC_DRIVER_NUM, 2, pid, 0);
  if (res.type != TOCK_SYSCALL_SUCCESS) {
    return tock_error_to_rcode(res.data[0]);
  } else {
    return TOCK_SUCCESS;
  }
}

int ipc_notify_client(int pid) {
  syscall_return_t res = command2(IPC_DRIVER_NUM, 3, pid, 0);
  if (res.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (res.type == TOCK_SYSCALL_FAILURE) {
    return tock_error_to_rcode(res.data[0]);
  } else {
    return TOCK_EBADRVAL;
  }
}

allow_rw_return_t ipc_share(int pid, void* base, int len) {
  return allow_readwrite(IPC_DRIVER_NUM, pid, base, len);
}

