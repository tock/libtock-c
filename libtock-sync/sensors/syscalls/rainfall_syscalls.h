#pragma once

#include <libtock/sensors/syscalls/rainfall_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a rainfall reading to complete.
returncode_t libtocksync_rainfall_yield_wait_for(uint32_t* rainfall);

#ifdef __cplusplus
}
#endif
