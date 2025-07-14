#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_RNG 0x40001

// Check if the RNG driver exists.
bool libtock_rng_driver_exists(void);

// Allows buffer to hold received randomness.
//
// - buffer: pointer to uint8_t array to store randomness
// - len: length of buffer.
returncode_t libtock_rng_set_allow_readwrite(uint8_t* buf, uint32_t len);

// Registers an upcall function that is called when requested randomness is
// obtained or provided buffer is full.
returncode_t libtock_rng_set_upcall(subscribe_upcall callback, void* opaque);

// Starts random number generator.
//
// - num_bytes: number of random bytes requested.
returncode_t libtock_rng_command_get_random(uint32_t num_bytes);

#ifdef __cplusplus
}
#endif
