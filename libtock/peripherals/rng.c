#include "rng.h"

// Internal upcall.
static void rng_upcall(__attribute__ ((unused)) int callback_type,
                       int                          received,
                       __attribute__ ((unused)) int val2,
                       void*                        opaque) {
  libtock_rng_callback cb = (libtock_rng_callback) opaque;
  cb(RETURNCODE_SUCCESS, received);
}

returncode_t libtock_rng_get_random_bytes(uint8_t* buf, uint32_t len, uint32_t num, libtock_rng_callback cb) {
  returncode_t ret;

  ret = libtock_rng_set_upcall(rng_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_rng_set_allow_readwrite(buf, len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_rng_command_get_random(num);
  return ret;
}
