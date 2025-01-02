#include "kv_syscalls.h"

#define TOCK_KV_CB           0

#define TOCK_KV_KEY_BUF      0
#define TOCK_KV_INPUT_BUF    1
#define TOCK_KV_OUTPUT_BUF   0

#define TOCK_KV_GET             1
#define TOCK_KV_SET             2
#define TOCK_KV_DELETE          3
#define TOCK_KV_ADD             4
#define TOCK_KV_UPDATE          5
#define TOCK_KV_GARBAGE_COLLECT 6

bool libtock_kv_exists(void) {
  return driver_exists(DRIVER_NUM_KV);
}

returncode_t libtock_kv_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_KV, TOCK_KV_CB, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_kv_set_readonly_allow_key_buffer(const uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_KV, TOCK_KV_KEY_BUF, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_kv_set_readonly_allow_input_buffer(const uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_KV, TOCK_KV_INPUT_BUF, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_kv_set_readwrite_allow_output_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_KV, TOCK_KV_OUTPUT_BUF, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

returncode_t libtock_kv_command_get(void) {
  syscall_return_t cval = command(DRIVER_NUM_KV, TOCK_KV_GET, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_kv_command_set(void) {
  syscall_return_t cval = command(DRIVER_NUM_KV, TOCK_KV_SET, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_kv_command_delete(void) {
  syscall_return_t cval = command(DRIVER_NUM_KV, TOCK_KV_DELETE, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_kv_command_garbage_collect(void) {
  syscall_return_t cval = command(DRIVER_NUM_KV, TOCK_KV_GARBAGE_COLLECT, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_kv_command_add(void) {
  syscall_return_t cval = command(DRIVER_NUM_KV, TOCK_KV_ADD, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_kv_command_update(void) {
  syscall_return_t cval = command(DRIVER_NUM_KV, TOCK_KV_UPDATE, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
