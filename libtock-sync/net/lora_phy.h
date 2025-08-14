#pragma once

#include <libtock/net/lora_phy.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_lora_phy_exists(void);

returncode_t libtocksync_lora_phy_write(const uint8_t* write,
                                        uint32_t       len);

returncode_t libtocksync_lora_phy_read_write(const uint8_t* write,
                                             uint8_t*       read,
                                             uint32_t       len);

#ifdef __cplusplus
}
#endif
