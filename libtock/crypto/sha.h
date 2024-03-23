#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for SHA hash callback.
//
// - `arg1` (`returncode_t`): Status from computing the hash.
typedef void (*libtock_sha_callback_hash)(returncode_t);

typedef enum {
	LIBTOCK_SHA256 = 0,
	LIBTOCK_SHA384 = 1,
	LIBTOCK_SHA512 = 2,
} libtock_sha_algorithm_t;



// Compute a SHA hash over `input_buffer` and store the hash in `hash_buffer`.
//
// The callback will be called when the hash is available.
returncode_t libtock_sha_simple_hash(libtock_sha_algorithm_t hash_type,
	uint8_t* input_buffer, uint32_t input_length,
	uint8_t* hash_buffer, uint32_t hash_length,
	libtock_sha_callback_hash cb);

#ifdef __cplusplus
}
#endif
