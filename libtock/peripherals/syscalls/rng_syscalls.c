#include "rng_syscalls.h"

bool libtock_rng_exists(void) {
  return driver_exists(DRIVER_NUM_RNG);
}

returncode_t libtock_rng_set_allow_readwrite(uint8_t* buf, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_RNG, 0, (void*) buf, len);
  return tock_allow_rw_return_to_returncode(aval);
}

returncode_t libtock_rng_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_RNG, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_rng_command_get_random(uint32_t num_bytes) {
  syscall_return_t cval = command(DRIVER_NUM_RNG, 1, num_bytes, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
