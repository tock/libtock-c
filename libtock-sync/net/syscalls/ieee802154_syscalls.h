#pragma once

#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

returncode_t libtocksync_ieee802154_yield_wait_for_receive(int* pans, int* dst_addr, int* src_addr);
returncode_t libtocksync_ieee802154_yield_wait_for_transmit(bool* acked);

#ifdef __cplusplus
}
#endif
