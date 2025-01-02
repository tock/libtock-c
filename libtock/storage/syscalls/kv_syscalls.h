#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_KV 0x50003

bool libtock_kv_exists(void);

returncode_t libtock_kv_set_upcall(subscribe_upcall callback, void* opaque);

returncode_t libtock_kv_set_readonly_allow_key_buffer(const uint8_t* buffer, uint32_t len);

returncode_t libtock_kv_set_readonly_allow_input_buffer(const uint8_t* buffer, uint32_t len);

returncode_t libtock_kv_set_readwrite_allow_output_buffer(uint8_t* buffer, uint32_t len);

returncode_t libtock_kv_command_get(void);

returncode_t libtock_kv_command_set(void);

returncode_t libtock_kv_command_delete(void);

returncode_t libtock_kv_command_garbage_collect(void);

returncode_t libtock_kv_command_add(void);

returncode_t libtock_kv_command_update(void);

#ifdef __cplusplus
}
#endif
