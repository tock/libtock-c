#pragma once

#include <libtock/net/syscalls/lora_phy_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a LoRa PHY SPI operation to complete.
returncode_t libtocksync_lora_phy_yield_wait_for(void);

#ifdef __cplusplus
}
#endif
