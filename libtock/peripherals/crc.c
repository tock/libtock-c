#include "crc.h"

static void crc_upcall(int status, int v1, __attribute__((unused)) int v2, void* opaque) {
  libtock_crc_callback_computed cb = (libtock_crc_callback_computed) opaque;
  cb(tock_status_to_returncode(status), v1);
}

returncode_t libtock_crc_compute(const uint8_t* buf, uint32_t buflen, libtock_crc_alg_t algorithm,
                                 libtock_crc_callback_computed cb) {
  returncode_t ret;

  ret = libtock_crc_set_readonly_allow(buf, buflen);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_crc_set_upcall(crc_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_crc_command_request(algorithm, buflen);
  return ret;
}
