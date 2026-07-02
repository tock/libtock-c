#pragma once

#include <libtock/sensors/syscalls/pressure_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a pressure reading to complete.
returncode_t libtocksync_pressure_yield_wait_for(int* pressure);

#ifdef __cplusplus
}
#endif
