#include "app_loader_syscalls.h"

returncode_t libtocksync_app_loader_yield_wait_for_setup(void) {
  yield_waitfor_return_t ywf;
  ywf = yield_wait_for(DRIVER_NUM_APP_LOADER, 0);
  return tock_status_to_returncode(ywf.data0);
}

returncode_t libtocksync_app_loader_yield_wait_for_write(void) {
  yield_waitfor_return_t ywf;
  ywf = yield_wait_for(DRIVER_NUM_APP_LOADER, 1);
  return tock_status_to_returncode(ywf.data0);
}

returncode_t libtocksync_app_loader_yield_wait_for_finalize(void) {
  yield_waitfor_return_t ywf;
  ywf = yield_wait_for(DRIVER_NUM_APP_LOADER, 2);
  return tock_status_to_returncode(ywf.data0);
}

returncode_t libtocksync_app_loader_yield_wait_for_load(void) {
  yield_waitfor_return_t ywf;
  ywf = yield_wait_for(DRIVER_NUM_APP_LOADER, 3);
  return tock_status_to_returncode(ywf.data0);
}

returncode_t libtocksync_app_loader_yield_wait_for_abort(void) {
  yield_waitfor_return_t ywf;
  ywf = yield_wait_for(DRIVER_NUM_APP_LOADER, 4);
  return tock_status_to_returncode(ywf.data0);
}
