#pragma once

#include <libtock/peripherals/syscalls/crc_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a CRC computation to complete.
returncode_t libtocksync_crc_yield_wait_for(uint32_t* crc);

#ifdef __cplusplus
}
#endif
