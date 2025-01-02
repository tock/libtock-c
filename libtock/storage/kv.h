#pragma once

#include "../tock.h"
#include "syscalls/kv_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for KV get callbacks.
//
// - `arg1` (`returncode_t`): Status of kv operation.
// - `arg2` (`int`): Length of get.
typedef void (*libtock_kv_callback_get)(returncode_t, int);

// Function signature for KV callbacks.
//
// - `arg1` (`returncode_t`): Status of kv operation.
typedef void (*libtock_kv_callback_done)(returncode_t);


returncode_t libtock_kv_get(const uint8_t* key_buffer, uint32_t key_len, uint8_t* ret_buffer, uint32_t ret_len,
                            libtock_kv_callback_get cb);

returncode_t libtock_kv_set(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer, uint32_t val_len,
                            libtock_kv_callback_done cb);

returncode_t libtock_kv_add(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer, uint32_t val_len,
                            libtock_kv_callback_done cb);

returncode_t libtock_kv_update(const uint8_t* key_buffer, uint32_t key_len, const uint8_t* val_buffer, uint32_t val_len,
                               libtock_kv_callback_done cb);

returncode_t libtock_kv_delete(const uint8_t* key_buffer, uint32_t key_len, libtock_kv_callback_done cb);

returncode_t libtock_kv_garbage_collect(libtock_kv_callback_done cb);

#ifdef __cplusplus
}
#endif
