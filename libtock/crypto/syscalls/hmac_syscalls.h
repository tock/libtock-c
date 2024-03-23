#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_HMAC 0x40003

bool libtock_hmac_exists(void);

returncode_t libtock_hmac_set_upcall(subscribe_upcall callback, void* opaque);

returncode_t libtock_hmac_set_readonly_allow_key_buffer(const uint8_t* buffer, uint32_t len);

returncode_t libtock_hmac_set_readonly_allow_data_buffer(const uint8_t* buffer, uint32_t len);

returncode_t libtock_hmac_set_readwrite_allow_destination_buffer(uint8_t* buffer, uint32_t len);

returncode_t libtock_hmac_command_set_algorithm(uint32_t hash);

returncode_t libtock_hmac_command_run(void);

returncode_t libtock_hmac_command_update(void);

returncode_t libtock_hmac_command_finish(void);

#ifdef __cplusplus
}
#endif
