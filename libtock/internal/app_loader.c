#include <stdio.h>
#include <string.h>

#include "tock.h"
#include "internal/app_loader.h"

int app_loader_exists(void) {
  syscall_return_t res = command(DRIVER_NUM_APP_LOADER, 0, 0, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int app_loader_subscribe(subscribe_upcall cb, void *userdata) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_APP_LOADER, 1, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

int app_loader_write_buffer(uint8_t *buffer, uint32_t len) {
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

int button_subscribe(subscribe_upcall callback, void *ud) {
  subscribe_return_t res = subscribe(DRIVER_NUM_BUTTON, 0, callback, ud);
  return tock_subscribe_return_to_returncode(res);
}

int button_count(int* count) {
  syscall_return_t res = command(DRIVER_NUM_BUTTON, 0, 0, 0);
  return tock_command_return_u32_to_returncode(res, (uint32_t*) count);
}

int button_enable_interrupt(int button_num) {
  syscall_return_t res = command(DRIVER_NUM_BUTTON, 1, button_num, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int button_disable_interrupt(int button_num) {
  syscall_return_t res = command(DRIVER_NUM_BUTTON, 2, button_num, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int button_read(int button_num, int* button_value) {
  syscall_return_t res = command(DRIVER_NUM_BUTTON, 3, button_num, 0);
  return tock_command_return_u32_to_returncode(res, (uint32_t*) button_value);
}
