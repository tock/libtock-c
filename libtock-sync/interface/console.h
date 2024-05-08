#pragma once

#include <libtock/tock.h>
#include <libtock/interface/console.h>

#ifdef __cplusplus
extern "C" {
#endif

returncode_t libtocksync_console_write(const uint8_t* buffer, uint32_t length, int* written);

returncode_t libtocksync_console_read(uint8_t* buffer, uint32_t length, int* read);

#ifdef __cplusplus
}
#endif
