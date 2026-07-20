#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_SHA 0x40005

bool libtock_sha_driver_exists(void);

returncode_t libtock_sha_set_hash_upcall(subscribe_upcall callback, void* opaque);

returncode_t libtock_sha_set_readonly_allow_data_buffer(uint8_t* buffer, uint32_t len);

returncode_t libtock_sha_set_readwrite_allow_destination_buffer(uint8_t* buffer, uint32_t len);

returncode_t libtock_sha_command_set_algorithm(uint8_t hash);

returncode_t libtock_sha_command_hash(void);

#ifdef __cplusplus
}
#endif
