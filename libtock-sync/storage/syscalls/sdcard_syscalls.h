#pragma once

#include <libtock/storage/syscalls/sdcard_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for an SD card initialization to complete.
returncode_t libtocksync_sdcard_yield_wait_for_initialize(uint32_t* block_size, uint32_t* size_in_kB);

// Wait for an SD card read or write operation to complete.
returncode_t libtocksync_sdcard_yield_wait_for_operation(void);

#ifdef __cplusplus
}
#endif
