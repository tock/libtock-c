#include <libtock/defer.h>
#include <libtock/peripherals/syscalls/crc_syscalls.h>

#include "crc.h"

#include "syscalls/crc_syscalls.h"

bool libtocksync_crc_exists(void) {
  return libtock_crc_driver_exists();
}

returncode_t libtocksync_crc_compute(const uint8_t* buf, size_t buflen, libtock_crc_alg_t algorithm, uint32_t* crc) {
  returncode_t ret;

  ret = libtock_crc_set_readonly_allow(buf, buflen);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_crc_set_readonly_allow(NULL, 0);
  }

  ret = libtock_crc_command_request((uint32_t) algorithm, buflen);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_crc_yield_wait_for(crc);
  return ret;
}
