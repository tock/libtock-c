#pragma once

#include <libtock/net/syscalls/ieee802154_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for an IEEE 802.15.4 frame transmission to complete.
returncode_t libtocksync_ieee802154_yield_wait_for_send(bool* acked);

// Wait for an IEEE 802.15.4 frame to be received.
returncode_t libtocksync_ieee802154_yield_wait_for_recv(void);

#ifdef __cplusplus
}
#endif
