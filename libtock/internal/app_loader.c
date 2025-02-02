#include "libtock/internal/app_loader.h"
#include "libtock/tock.h"

int app_loader_exists(void) {
  syscall_return_t res = command(DRIVER_NUM_APP_LOADER, 0, 0, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int app_loader_setup_subscribe(subscribe_upcall cb, void* userdata) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_APP_LOADER, 0, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

int app_loader_write_subscribe(subscribe_upcall cb, void* userdata) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_APP_LOADER, 1, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

int app_loader_load_subscribe(subscribe_upcall cb, void* userdata) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_APP_LOADER, 2, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

int app_loader_write_buffer(uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_APP_LOADER, 0, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

int app_loader_command_setup(uint32_t app_length) {
  syscall_return_t res = command(DRIVER_NUM_APP_LOADER, 1, app_length, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int app_loader_command_write(uint32_t flash_offset, uint32_t write_length) {
  syscall_return_t res = command(DRIVER_NUM_APP_LOADER, 2, flash_offset, write_length);
  return tock_command_return_novalue_to_returncode(res);
}

int app_loader_command_load(void) {
  syscall_return_t res = command(DRIVER_NUM_APP_LOADER, 3, 0, 0);
  return tock_command_return_novalue_to_returncode(res);
}
