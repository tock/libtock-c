#include "hmac.h"

// would be in a libtock-sync/support.h or internal.h or similar
#define SCOPED_ALLOW(method, buffer, length) \
  inline void unallow_##buffer(returncode_t* sentinel##buffer) { if (sentinel##buffer == RETURNCODE_SUCCESS) method(NULL, 0); } \
  returncode_t ret_##buffer __attribute__((cleanup(unallow_##buffer))) = method(buffer, length);  \

returncode_t libtocksync_hmac_simple(libtock_hmac_algorithm_t hmac_type,
                                     uint8_t* key_buffer, uint32_t key_length,
                                     uint8_t* input_buffer, uint32_t input_length,
                                     uint8_t* hmac_buffer, uint32_t hmac_length) {
  returncode_t ret;

  ret = libtock_hmac_command_set_algorithm((uint32_t) hmac_type);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Macro expands to this:
  //
  // inline void unallow_key_buffer(returncode_t* sentinel_key_buffer) { if (sentinel_key_buffer == RETURNCODE_SUCCESS) libtock_hmac_set_readonly_allow_key_buffer(NULL, 0); }
  // returncode_t ret_key_buffer __attribute__((cleanup(unallow_key_buffer))) = libtock_hmac_set_readonly_allow_key_buffer(key_buffer, key_length);

  SCOPED_ALLOW(libtock_hmac_set_readonly_allow_key_buffer, key_buffer, key_length);
  if (ret_key_buffer != RETURNCODE_SUCCESS) return ret_key_buffer;

  SCOPED_ALLOW(libtock_hmac_set_readonly_allow_data_buffer, input_buffer, input_length);
  if (ret_input_buffer != RETURNCODE_SUCCESS) return ret_input_buffer;

  SCOPED_ALLOW(libtock_hmac_set_readwrite_allow_destination_buffer, hmac_buffer, hmac_length);
  if (ret_hmac_buffer != RETURNCODE_SUCCESS) return ret_hmac_buffer;

  ret = libtock_hmac_command_run();
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for the operation.
  ret = libtocksync_hmac_yield_wait_for();

  return ret;
}
