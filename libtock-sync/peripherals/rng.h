#pragma once

#include <tock.h>
#include <libtock/peripherals/rng.h>

#ifdef __cplusplus
extern "C" {
#endif

// Synchronous RNG request.
//
// Request `num` random bytes.
//
// -  buf: buffer to store random bytes in.
// -  len: length of buffer.
// -  num: number of random bytes requested.
// -  num_received: pointer which will be set with number of bytes received.
returncode_t libtocksync_rng_get_random_bytes(uint8_t* buf, uint32_t len, uint32_t num, int* num_received);

#ifdef __cplusplus
}
#endif
