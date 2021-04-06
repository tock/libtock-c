#include "ipc.h"
#include "tock.h"

int ipc_discover(const char* pkg_name, int* svc_id) {
  int len = strlen(pkg_name);

  allow_ro_return_t prev = allow_readonly(IPC_DRIVER_NUM, 0, pkg_name, len);
  if (!prev.success) return tock_status_to_returncode(prev.status);

  syscall_return_t res = command(IPC_DRIVER_NUM, 1, 0, 0);
  int ret = tock_command_return_u32_to_returncode(res, (uint32_t*) svc_id);
  if (ret < 0) return ret;

  prev = allow_readonly(IPC_DRIVER_NUM, 0, prev.ptr, prev.size);
  if (!prev.success) return tock_status_to_returncode(prev.status);

  return RETURNCODE_SUCCESS;
}

int ipc_register_service_callback(subscribe_upcall callback, void *ud) {
  subscribe_return_t sval = subscribe(IPC_DRIVER_NUM, 0, callback, ud);
  return tock_subscribe_return_to_returncode(sval);
}

int ipc_register_client_callback(int svc_id, subscribe_upcall callback, void *ud) {
  if (svc_id <= 0) {
    return RETURNCODE_FAIL;
  }
  subscribe_return_t sval = subscribe(IPC_DRIVER_NUM, svc_id, callback, ud);
  return tock_subscribe_return_to_returncode(sval);
}

int ipc_notify_service(int pid) {
  syscall_return_t res = command(IPC_DRIVER_NUM, 2, pid, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int ipc_notify_client(int pid) {
  syscall_return_t res = command(IPC_DRIVER_NUM, 3, pid, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int ipc_share(int pid, void* base, int len) {
  allow_rw_return_t aval = allow_readwrite(IPC_DRIVER_NUM, pid, base, len);
  return tock_allow_rw_return_to_returncode(aval);
}
