#include "sha.h"

static void sha_upcall(int ret,
                       __attribute__ ((unused)) int unused1,
                       __attribute__ ((unused)) int unused2, void* opaque) {
  libtock_sha_callback_hash cb = (libtock_sha_callback_hash) opaque;
  cb((returncode_t) ret);
}

returncode_t libtock_sha_simple_hash(libtock_sha_algorithm_t hash_type,
                                     uint8_t* input_buffer, uint32_t input_length,
                                     uint8_t* hash_buffer, uint32_t hash_length,
                                     libtock_sha_callback_hash cb) {

  returncode_t ret;

  ret = libtock_sha_command_set_algorithm((uint8_t) hash_type);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_sha_set_readonly_allow_data_buffer(input_buffer, input_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_sha_set_readwrite_allow_destination_buffer(hash_buffer, hash_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_sha_set_upcall(sha_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_sha_command_run();
  return ret;
}
