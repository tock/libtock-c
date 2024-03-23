#pragma once

#include <tock.h>
#include <crypto/sha.h>

#ifdef __cplusplus
extern "C" {
#endif

// Compute a SHA hash on the given buffer.
returncode_t libtocksync_sha_simple_hash(libtock_sha_algorithm_t hash_type,
  uint8_t* input_buffer, uint32_t input_length,
  uint8_t* hash_buffer, uint32_t hash_length);

#ifdef __cplusplus
}
#endif
