#include "app_state_syscalls.h"

returncode_t libtocksync_app_state_yield_wait_for(void) {
  yield_wait_for(DRIVER_NUM_APP_STATE, 0);
  return RETURNCODE_SUCCESS;
}
