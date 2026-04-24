#include <libtock/defer.h>
#include <libtock/peripherals/syscalls/rng_syscalls.h>

#include "rng.h"

#include "syscalls/rng_syscalls.h"

bool libtocksync_rng_exists(void) {
  return libtock_rng_driver_exists();
}

returncode_t libtocksync_rng_get_random_bytes(uint8_t* buf, uint32_t len, uint32_t num, int* num_received) {
  returncode_t ret;

  ret = libtock_rng_set_allow_readwrite(buf, len);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_rng_set_allow_readwrite(NULL, 0);
  }

  ret = libtock_rng_command_get_random(num);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_rng_yield_wait_for(num_received);
  return ret;
}
