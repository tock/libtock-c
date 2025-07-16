#include <libtock/crypto/syscalls/sha_syscalls.h>

#include "sha.h"

struct sha_data {
  bool fired;
  returncode_t ret;
};

static struct sha_data result = {.fired = false};

static void sha_cb_hash(returncode_t ret) {
  result.fired = true;
  result.ret   = ret;
}

bool libtocksync_sha_exists(void) {
  return libtocksync_sha_driver_exists();
}

returncode_t libtocksync_sha_simple_hash(libtock_sha_algorithm_t hash_type,
                                         uint8_t* input_buffer, uint32_t input_length,
                                         uint8_t* hash_buffer, uint32_t hash_length) {
  returncode_t ret;

  result.fired = false;

  ret = libtock_sha_simple_hash(hash_type, input_buffer, input_length, hash_buffer, hash_length, sha_cb_hash);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for the callback.
  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  ret = libtock_sha_set_readonly_allow_data_buffer(NULL, 0);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_sha_set_readwrite_allow_destination_buffer(NULL, 0);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return RETURNCODE_SUCCESS;
}
