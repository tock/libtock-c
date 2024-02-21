#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

int kv_set_callback(subscribe_upcall callback, void* callback_args);

int kv_set_key_buffer(const uint8_t* buffer, uint32_t len);
int kv_set_input_buffer(const uint8_t* buffer, uint32_t len);
int kv_set_output_buffer(uint8_t* buffer, uint32_t len);

/*
 * Check that the KV system exists
 */
int kv_check_status(void);

int kv_get(void);
int kv_set(void);
int kv_add(void);
int kv_update(void);
int kv_delete(void);

int kv_get_sync(const uint8_t* key_buffer, uint32_t key_len, uint8_t* ret_buffer, uint32_t ret_len, uint32_t* value_len);
int kv_set_sync(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer, uint32_t val_len);
int kv_add_sync(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer, uint32_t val_len);
int kv_update_sync(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer, uint32_t val_len);
int kv_delete_sync(const uint8_t* key_buffer, uint32_t key_len);

#ifdef __cplusplus
}
#endif
