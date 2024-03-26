#pragma once

#include "tock.h"
#include "syscalls/aes_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  LIBTOCK_AES128Ctr = 0,
  LIBTOCK_AES128CBC = 1,
  LIBTOCK_AES128ECB = 2,
  LIBTOCK_AES128CCM = 3
} libtock_aes_algorithm_t;

// Set the AES algorithm
// operation:
//    * 0 -> AES128Ctr
//    * 1 -> AES128CBC
//    * 2 -> AES128ECB
//    * 3 -> AES128CCM
// encrypting:
//    * true -> Encrypt the source data
//    * false -> Decrypt the source data
returncode_t libtock_aes_set_algorithm(libtock_aes_algorithm_t operation, bool encrypting);

// Setup the platform and run the first encryption. This must be called before
// `aes_crypt()` and `aes_finish()` but must be called after
// `aes_set_algorithm()` and all relevant buffers are set up.
//
// This will load in the specified key, IV and then run the
// encryption/decryption operation on the data in the source buffer and save the
// output in the destination buffer.
//
// This will trigger a callback.
returncode_t libtock_aes_setup(void);

// This must be called after aes_setup(). It will run the encryption/decryption
// operation on the source data and store it in the destination buffer, based on
// the key and IV that was loaded by `aes_setup()`.
//
// Between a call of `aes_setup()` and `aes_finish()` this can be called any
// number of times (from 0 to infinite).
//
// Each call to this will trigger a callback.
returncode_t libtock_aes_crypt(void);

// This must be called when the operation is complete. This indicates that the
// app has finished the operation. Any future operations need to call
// `aes_set_algorithm()` and `aes_setup()` as the key and IV will be cleared.
returncode_t libtock_aes_finish(void);

// This sets the A offset for AES CCM.
returncode_t libtock_aes_ccm_set_a_off(uint32_t value);

// This sets the M offset for AES CCM.
returncode_t libtock_aes_ccm_set_m_off(uint32_t value);

// This sets the mic length for AES CCM.
returncode_t libtock_aes_ccm_set_mic_len(uint32_t value);

// This sets the confidential bool for AES CCM.
returncode_t libtock_aes_ccm_set_confidential(bool value);

#ifdef __cplusplus
}
#endif
