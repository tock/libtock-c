#include "sha.h"

returncode_t libtocksync_sha_simple_hash(libtock_sha_algorithm_t hash_type,
                                         uint8_t* input_buffer, uint32_t input_length,
                                         uint8_t* hash_buffer, uint32_t hash_length) {
  returncode_t ret;

  ret = libtock_sha_command_set_algorithm((uint8_t) hash_type);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_sha_set_readonly_allow_data_buffer(input_buffer, input_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_sha_set_readwrite_allow_destination_buffer(hash_buffer, hash_length);
  if (ret != RETURNCODE_SUCCESS) goto exit1;

  ret = libtock_sha_command_run();
  if (ret != RETURNCODE_SUCCESS) goto exit2;

  // Wait for the operation.
  ret = libtocksync_sha_yield_wait_for();

exit2:
  libtock_sha_set_readwrite_allow_destination_buffer(NULL, 0);

exit1:
  libtock_sha_set_readonly_allow_data_buffer(NULL, 0);

  return ret;
}
