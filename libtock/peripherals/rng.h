#pragma once

#include "tock.h"
#include "syscalls/rng_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for callbacks when randomness is available.
//
// - `arg1` (`returncode_t`): Returncode indicating status of the RNG call.
// - `arg2` (`int`): Number of random bytes available.
typedef void (*libtock_rng_callback)(returncode_t, int);

// Get random bytes.
//
// Asynchronous RNG request. Registers callback and buffer and starts RNG.
//
// - buf: buffer to store the random bytes in.
// - len: length of buffer.
// - num: number of random bytes requested.
// - cb: callback when random bytes are available.
returncode_t libtock_rng_get_random_bytes(uint8_t* buf, uint32_t len, uint32_t num, libtock_rng_callback cb);

#ifdef __cplusplus
}
#endif
