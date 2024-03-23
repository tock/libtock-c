#include "hmac.h"

struct hmac_data {
  bool fired;
  returncode_t ret;
};

static struct hmac_data result = {.fired = false};

static void hmac_cb_hmac(returncode_t ret) {
  result.fired = true;
  result.ret   = ret;
}

returncode_t libtocksync_hmac_simple(libtock_hmac_algorithm_t hmac_type,
                                     uint8_t* key_buffer, uint32_t key_length,
                                     uint8_t* input_buffer, uint32_t input_length,
                                     uint8_t* hmac_buffer, uint32_t hmac_length) {
  returncode_t ret;

  result.fired = false;

  ret = libtock_hmac_simple(hmac_type, key_buffer, key_length, input_buffer, input_length, hmac_buffer, hmac_length,
                            hmac_cb_hmac);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for the callback.
  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  ret = libtock_hmac_set_readonly_allow_key_buffer(NULL, 0);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_hmac_set_readonly_allow_data_buffer(NULL, 0);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_hmac_set_readwrite_allow_destination_buffer(NULL, 0);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return RETURNCODE_SUCCESS;
}
