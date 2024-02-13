#include <stdio.h>
#include <string.h>

#include "tock.h"
#include "internal/app_loader.h"

int app_loader_exists(void) {
  syscall_return_t res = command(DRIVER_NUM_APP_LOADER, 0, 0, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int app_loader_subscribe(subscribe_upcall cb, void *userdata) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_APP_LOADER, 0, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

int app_loader_write_buffer(uint8_t *buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_APP_LOADER, 0, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

int app_loader_command_setup(int app_length) {
  syscall_return_t res = command(DRIVER_NUM_APP_LOADER, 1, app_length, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int app_loader_command_write(int write_length) {
  syscall_return_t res = command(DRIVER_NUM_APP_LOADER, 2, write_length, 0);
  return tock_command_return_novalue_to_returncode(res);
}
