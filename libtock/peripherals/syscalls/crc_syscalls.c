#include "crc_syscalls.h"

bool libtock_crc_exists(void) {
  return driver_exists(DRIVER_NUM_CRC);
}

returncode_t libtock_crc_set_upcall(subscribe_upcall callback, void *opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_CRC, 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_crc_set_readonly_allow(const uint8_t* buf, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_CRC, 0, (void*) buf, len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_crc_command_request(uint32_t algorithm, uint32_t len) {
  syscall_return_t cval = command(DRIVER_NUM_CRC, 1, algorithm, len);
  return tock_command_return_novalue_to_returncode(cval);
}
