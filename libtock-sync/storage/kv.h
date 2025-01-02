#pragma once

#include <libtock/storage/kv.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

returncode_t libtocksync_kv_get(const uint8_t* key_buffer, uint32_t key_len, uint8_t* ret_buffer, uint32_t ret_len,
                                uint32_t* value_len);

returncode_t libtocksync_kv_set(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer,
                                uint32_t val_len);

returncode_t libtocksync_kv_add(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer,
                                uint32_t val_len);

returncode_t libtocksync_kv_update(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer,
                                   uint32_t val_len);

returncode_t libtocksync_kv_delete(const uint8_t* key_buffer, uint32_t key_len);

returncode_t libtocksync_kv_garbage_collect(void);

#ifdef __cplusplus
}
#endif
