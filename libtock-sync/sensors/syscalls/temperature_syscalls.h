#pragma once

#include <libtock/sensors/syscalls/temperature_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a temperature read to finish.
returncode_t libtocksync_temperature_yield_wait_for(int* temp);

#ifdef __cplusplus
}
#endif
