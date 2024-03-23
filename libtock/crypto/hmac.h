#pragma once

#include "tock.h"
#include "syscalls/hmac_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for HMAC callback.
//
// - `arg1` (`returncode_t`): Status from computing the HMAC.
typedef void (*libtock_hmac_callback_hmac)(returncode_t);

typedef enum {
	LIBTOCK_HMAC_SHA256 = 0,
	LIBTOCK_HMAC_SHA384 = 1,
	LIBTOCK_HMAC_SHA512 = 2,
} libtock_hmac_algorithm_t;



// Compute an HMAC using `keyb_buffer` over `input_buffer` and store the result
// in `hash_buffer`.
//
// The callback will be called when the HMAC is available.
returncode_t libtock_hmac_simple(libtock_hmac_algorithm_t hmac_type,
  uint8_t* key_buffer, uint32_t key_length,
                                     uint8_t* input_buffer, uint32_t input_length,
                                     uint8_t* hash_buffer, uint32_t hash_length,
                                     libtock_hmac_callback_hmac cb);

#ifdef __cplusplus
}
#endif





