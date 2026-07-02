#pragma once

#include <libtock/sensors/syscalls/ambient_light_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for an ambient light intensity reading to complete.
returncode_t libtocksync_ambient_light_yield_wait_for(int* intensity);

#ifdef __cplusplus
}
#endif
