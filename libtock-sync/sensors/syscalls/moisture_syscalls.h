#pragma once

#include <libtock/sensors/syscalls/moisture_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a moisture reading to complete.
returncode_t libtocksync_moisture_yield_wait_for(int* moisture);

#ifdef __cplusplus
}
#endif
