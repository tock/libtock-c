#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_AES 0x40006

bool libtock_aes_driver_exists(void);

returncode_t libtock_aes_set_upcall(subscribe_upcall callback, void* opaque);

returncode_t libtock_aes_set_readonly_allow_key_buffer(const uint8_t* buffer, uint32_t len);

// Contains the IV buffer, if applicable.
// If doing a AES CCM operation this contains the nonce instead.
returncode_t libtock_aes_set_readonly_allow_iv_buffer(const uint8_t* buffer, uint32_t len);

// Contains the nonce for CCM buffer.
returncode_t libtock_aes_set_readonly_allow_nonce_buffer(const uint8_t* buffer, uint32_t len);

// Contains the source buffer.
// If doing a AES CCM operation this sets the mlen value to `len`.
returncode_t libtock_aes_set_readonly_allow_source_buffer(const uint8_t* buffer, uint32_t len);

returncode_t libtock_aes_set_readwrite_allow_dest_buffer(uint8_t* buffer, uint32_t len);

returncode_t libtock_aes_command_set_algorithm(uint8_t operation, bool encrypting);

returncode_t libtock_aes_command_setup(void);

returncode_t libtock_aes_command_crypt(void);

returncode_t libtock_aes_command_finish(void);

returncode_t libtock_aes_command_ccm_set_a_off(uint32_t value);

returncode_t libtock_aes_command_ccm_set_m_off(uint32_t value);

returncode_t libtock_aes_command_ccm_set_mic_len(uint32_t value);

returncode_t libtock_aes_command_ccm_set_confidential(bool value);

#ifdef __cplusplus
}
#endif
