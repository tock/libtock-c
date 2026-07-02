#pragma once

#include <libtock/sensors/syscalls/proximity_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a proximity reading to complete.
returncode_t libtocksync_proximity_yield_wait_for(uint8_t* proximity);

#ifdef __cplusplus
}
#endif
