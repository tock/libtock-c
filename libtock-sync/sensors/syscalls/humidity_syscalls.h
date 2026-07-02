#pragma once

#include <libtock/sensors/syscalls/humidity_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a humidity reading to complete.
returncode_t libtocksync_humidity_yield_wait_for(int* humidity);

#ifdef __cplusplus
}
#endif
