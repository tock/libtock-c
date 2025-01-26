#pragma once

#include <libtock/crypto/hmac.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Compute an HMAC on the given buffer.
returncode_t libtocksync_hmac_simple(libtock_hmac_algorithm_t hmac_type,
                                     uint8_t* key_buffer, uint32_t key_length,
                                     uint8_t* input_buffer, uint32_t input_length,
                                     uint8_t* hmac_buffer, uint32_t hmac_length);

#ifdef __cplusplus
}
#endif
