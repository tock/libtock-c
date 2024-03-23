#include "sha_syscalls.h"

#define TOCK_SHA_CB       0

#define TOCK_SHA_DATA_BUF     1
#define TOCK_SHA_DEST_BUF     2

#define TOCK_SHA_SET_ALGORITHM   0
#define TOCK_SHA_RUN             1
#define TOCK_SHA_UPDATE          2
#define TOCK_SHA_FINISH          3

bool libtock_sha_exists(void) {
  return driver_exists(DRIVER_NUM_SHA);
}

returncode_t libtock_sha_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_SHA, TOCK_SHA_CB, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_sha_set_readonly_allow_data_buffer(uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_SHA, TOCK_SHA_DATA_BUF, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_sha_set_readwrite_allow_destination_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_SHA, TOCK_SHA_DEST_BUF, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

returncode_t libtock_sha_command_set_algorithm(uint8_t hash) {
  syscall_return_t cval = command(DRIVER_NUM_SHA, TOCK_SHA_SET_ALGORITHM, hash, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_sha_command_run(void) {
  syscall_return_t cval = command(DRIVER_NUM_SHA, TOCK_SHA_RUN, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_sha_command_update(void) {
  syscall_return_t cval = command(DRIVER_NUM_SHA, TOCK_SHA_UPDATE, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_sha_command_finish(void) {
  syscall_return_t cval = command(DRIVER_NUM_SHA, TOCK_SHA_FINISH, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
