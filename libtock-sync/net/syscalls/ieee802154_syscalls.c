#include "ieee802154_syscalls.h"

returncode_t libtocksync_ieee802154_yield_wait_for_send(bool* acked) {
  yield_waitfor_return_t ywf;
  returncode_t ret;
  ywf = yield_wait_for(DRIVER_NUM_IEEE802154, SUBSCRIBE_TX);
  ret = (returncode_t) ywf.data0;
  if (ret != RETURNCODE_SUCCESS) return ret;
  *acked = (bool) ywf.data1;
  return ret;
}

returncode_t libtocksync_ieee802154_yield_wait_for_recv(void) {
  yield_wait_for(DRIVER_NUM_IEEE802154, SUBSCRIBE_RX);
  return RETURNCODE_SUCCESS;
}
