#pragma once

#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

returncode_t libtocksync_console_write(const uint8_t* buffer, uint32_t length, uint32_t* written);

returncode_t libtocksync_console_read(uint8_t* buffer, uint32_t length, uint32_t* read);

#ifdef __cplusplus
}
#endif
