#include "libtock/internal/in_place_process_loader.h"
#include "libtock/tock.h"

int in_place_process_loader_exists(void) {
  syscall_return_t res = command(DRIVER_NUM_IN_PLACE_PROCESS_LOADER, 0, 0, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int in_place_process_loader_load_subscribe(subscribe_upcall cb, void* userdata) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_IN_PLACE_PROCESS_LOADER, 0, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

int in_place_process_loader_command_load(uint32_t app_address, uint32_t app_size) {
  syscall_return_t res = command(DRIVER_NUM_IN_PLACE_PROCESS_LOADER, 1, app_address, app_size);
  return tock_command_return_novalue_to_returncode(res);
}