#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TOCK_HMAC_ALG_SHA256    0
#define TOCK_HMAC_ALG_SHA384    1
#define TOCK_HMAC_ALG_SHA512    2

int hmac_set_callback (subscribe_upcall callback, void* callback_args);

int hmac_set_key_buffer(const uint8_t* buffer, uint32_t len);
int hmac_set_data_buffer(const uint8_t* buffer, uint32_t len);
int hmac_set_dest_buffer(uint8_t* buffer, uint32_t len);

int hmac_set_algorithm(uint8_t hash);
int hmac_run(void);
int hmac_update(void);
int hmac_finish(void);

#ifdef __cplusplus
}
#endif
