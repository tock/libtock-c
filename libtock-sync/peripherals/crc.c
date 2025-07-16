#include <libtock/peripherals/syscalls/crc_syscalls.h>

#include "crc.h"

struct crc_data {
  bool fired;
  int status;
  uint32_t crc;
};

static struct crc_data result = {.fired = false};

static void crc_callback(returncode_t ret, uint32_t crc) {
  result.fired  = true;
  result.status = ret;
  result.crc    = crc;
}

bool libtocksync_crc_exists(void) {
  return libtock_crc_driver_exists();
}

returncode_t libtocksync_crc_compute(const uint8_t* buf, size_t buflen, libtock_crc_alg_t algorithm, uint32_t* crc) {
  returncode_t ret;
  result.fired = false;

  ret = libtock_crc_compute(buf, buflen, algorithm, crc_callback);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&result.fired);
  if (result.status != RETURNCODE_SUCCESS) return result.status;

  ret = libtock_crc_set_readonly_allow(NULL, 0);
  if (ret != RETURNCODE_SUCCESS) return ret;

  *crc = result.crc;

  return RETURNCODE_SUCCESS;
}
