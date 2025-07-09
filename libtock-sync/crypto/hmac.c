#include "hmac.h"

returncode_t libtocksync_hmac_simple(libtock_hmac_algorithm_t hmac_type,
                                     uint8_t* key_buffer, uint32_t key_length,
                                     uint8_t* input_buffer, uint32_t input_length,
                                     uint8_t* hmac_buffer, uint32_t hmac_length) {
  returncode_t ret;

  ret = libtock_hmac_command_set_algorithm((uint32_t) hmac_type);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_hmac_set_readonly_allow_key_buffer(key_buffer, key_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_hmac_set_readonly_allow_data_buffer(input_buffer, input_length);
  if (ret != RETURNCODE_SUCCESS) goto exit1;

  ret = libtock_hmac_set_readwrite_allow_destination_buffer(hmac_buffer, hmac_length);
  if (ret != RETURNCODE_SUCCESS) goto exit2;

  ret = libtock_hmac_command_run();
  if (ret != RETURNCODE_SUCCESS) goto exit3;

  // Wait for the operation.
  ret = libtocksync_hmac_yield_wait_for();

exit3:
  libtock_hmac_set_readwrite_allow_destination_buffer(NULL, 0);

exit2:
  libtock_hmac_set_readonly_allow_data_buffer(NULL, 0);

exit1:
  libtock_hmac_set_readonly_allow_key_buffer(NULL, 0);

  return ret;
}
