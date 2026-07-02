#include <libtock/storage/app_state.h>

#include "app_state.h"

#include "syscalls/app_state_syscalls.h"

static void app_state_noop_cb(__attribute__ ((unused)) returncode_t ret) {}

bool libtocksync_app_state_exists(void) {
  return libtock_app_state_driver_exists();
}

returncode_t libtocksync_app_state_save(void) {
  returncode_t err;

  err = libtock_app_state_save(app_state_noop_cb);
  if (err != RETURNCODE_SUCCESS) return err;

  return libtocksync_app_state_yield_wait_for();
}
