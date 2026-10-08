#include "ipc_registry_package_name_syscalls.h"

returncode_t libtocksync_ipc_registry_package_name_yield_wait_for_registration_complete(void) {
  yield_waitfor_return_t ywf = yield_wait_for(DRIVER_NUM_IPC_REGISTRY_PACKAGE_NAME, 0);
  return tock_status_to_returncode(ywf.data0);
}

returncode_t libtocksync_ipc_registry_package_name_yield_wait_for_discovery_complete(uint64_t* ipc_id) {
  yield_waitfor_return_t ywf = yield_wait_for(DRIVER_NUM_IPC_REGISTRY_PACKAGE_NAME, 1);

  uint32_t lower = ywf.data1;
  uint32_t upper = ywf.data2;
  *ipc_id = (((uint64_t)upper) << 32) | ((uint64_t)lower);

  return tock_status_to_returncode(ywf.data0);
}
