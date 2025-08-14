#include <libtock/net/syscalls/ieee802154_syscalls.h>

#include "ieee802154_syscalls.h"


returncode_t libtocksync_ieee802154_yield_wait_for_receive(int* pans, int* dst_addr, int* src_addr) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_IEEE802154, 0);

  *pans = ret.data0;
  *dst_addr = ret.data1;
  *src_addr = ret.data2;

  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_ieee802154_yield_wait_for_transmit(bool* acked) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_IEEE802154, 1);

  *acked = ret.data1 == 1;

  return tock_status_to_returncode(ret.data0);
}
