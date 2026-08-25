#include "ipc_relay_request_syscalls.h"

returncode_t libtocksync_ipc_relay_request_yield_wait_for_client_response_received(size_t* response_len) {
  yield_waitfor_return_t ywf = yield_wait_for(DRIVER_NUM_IPC_RELAY_REQUEST, 0);
  *response_len = (size_t)ywf.data1;
  return tock_status_to_returncode(ywf.data0);
}

returncode_t libtocksync_ipc_relay_request_yield_wait_for_server_request_waiting(uint64_t* ipc_id) {
  yield_waitfor_return_t ywf = yield_wait_for(DRIVER_NUM_IPC_RELAY_REQUEST, 1);

  uint32_t lower = ywf.data1;
  uint32_t upper = ywf.data2;
  *ipc_id = (((uint64_t)upper) << 32) | ((uint64_t)lower);

  return tock_status_to_returncode(ywf.data0);
}
