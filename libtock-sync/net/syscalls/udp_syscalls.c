#include "udp_syscalls.h"

returncode_t libtocksync_udp_yield_wait_for_send(void) {
  yield_waitfor_return_t ywf;
  ywf = yield_wait_for(DRIVER_NUM_UDP, SUBSCRIBE_TX);
  return (returncode_t) ywf.data0;
}

returncode_t libtocksync_udp_yield_wait_for_recv(size_t* length) {
  yield_waitfor_return_t ret;
  ret     = yield_wait_for(DRIVER_NUM_UDP, SUBSCRIBE_RX);
  *length = (size_t) ret.data0;
  return RETURNCODE_SUCCESS;
}
