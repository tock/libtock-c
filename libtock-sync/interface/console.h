#pragma once

#include <libtock/interface/console.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_console_exists(void);

returncode_t libtocksync_console_write(const uint8_t* buffer, uint32_t length, int* written);

returncode_t libtocksync_console_read(uint8_t* buffer, uint32_t length, int* read);

#ifdef __cplusplus
}
#endif
