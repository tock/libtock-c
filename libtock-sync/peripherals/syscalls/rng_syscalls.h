#pragma once

#include <libtock/peripherals/syscalls/rng_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a random bytes request to complete.
returncode_t libtocksync_rng_yield_wait_for(int* received);

#ifdef __cplusplus
}
#endif
