#include "internal/nonvolatile_storage.h"

subscribe_return_t nonvolatile_storage_internal_read_done_subscribe(subscribe_upcall cb, void *userdata) {
  return subscribe2(DRIVER_NUM_NONVOLATILE_STORAGE, 0, cb, userdata);
}

subscribe_return_t nonvolatile_storage_internal_write_done_subscribe(subscribe_upcall cb, void *userdata) {
  return subscribe2(DRIVER_NUM_NONVOLATILE_STORAGE, 1, cb, userdata);
}

allow_rw_return_t nonvolatile_storage_internal_read_buffer(uint8_t* buffer, uint32_t len) {
  return allow_readwrite(DRIVER_NUM_NONVOLATILE_STORAGE, 0, (void*) buffer, len);
}

allow_ro_return_t nonvolatile_storage_internal_write_buffer(uint8_t* buffer, uint32_t len) {
  return allow_readonly(DRIVER_NUM_NONVOLATILE_STORAGE, 0, (void*) buffer, len);
}

int nonvolatile_storage_internal_get_number_bytes(void) {
  syscall_return_t res = command2(DRIVER_NUM_NONVOLATILE_STORAGE, 1, 0, 0);
  if (res.type == TOCK_SYSCALL_SUCCESS_U32) {
    return res.data[0];
  } else if (res.type == TOCK_SYSCALL_FAILURE) {
    return tock_error_to_rcode(res.data[0]);
  } else {
    return TOCK_EBADRVAL;
  }
}

int nonvolatile_storage_internal_read(uint32_t offset, uint32_t length) {
  syscall_return_t res =
    command2(DRIVER_NUM_NONVOLATILE_STORAGE, 2, (int) offset, (int) length);
  if (res.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (res.type == TOCK_SYSCALL_FAILURE) {
    return tock_error_to_rcode(res.data[0]);
  } else {
    return TOCK_EBADRVAL;
  }
}

int nonvolatile_storage_internal_write(uint32_t offset, uint32_t length) {
  syscall_return_t res =
    command2(DRIVER_NUM_NONVOLATILE_STORAGE, 3, (int) offset, (int) length);
  if (res.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (res.type == TOCK_SYSCALL_FAILURE) {
    return tock_error_to_rcode(res.data[0]);
  } else {
    return TOCK_EBADRVAL;
  }
}
