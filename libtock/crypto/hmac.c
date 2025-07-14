#include "hmac.h"
#include "syscalls/hmac_syscalls.h"

static void hmac_upcall(int ret,
                        __attribute__ ((unused)) int unused1,
                        __attribute__ ((unused)) int unused2, void* opaque) {
  libtock_hmac_callback_hmac cb = (libtock_hmac_callback_hmac) opaque;
  cb(tock_status_to_returncode(ret));
}

bool libtock_hmac_exists(void) {
  return libtock_hmac_driver_exists();
}

returncode_t libtock_hmac_simple(libtock_hmac_algorithm_t hmac_type,
                                 uint8_t* key_buffer, uint32_t key_length,
                                 uint8_t* input_buffer, uint32_t input_length,
                                 uint8_t* hmac_buffer, uint32_t hmac_length,
                                 libtock_hmac_callback_hmac cb) {

  returncode_t ret;

  ret = libtock_hmac_command_set_algorithm((uint32_t) hmac_type);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_hmac_set_readonly_allow_key_buffer(key_buffer, key_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_hmac_set_readonly_allow_data_buffer(input_buffer, input_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_hmac_set_readwrite_allow_destination_buffer(hmac_buffer, hmac_length);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_hmac_set_upcall(hmac_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_hmac_command_run();
  return ret;
}
