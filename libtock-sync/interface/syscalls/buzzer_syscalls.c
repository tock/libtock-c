#include "buzzer_syscalls.h"

returncode_t libtocksync_buzzer_yield_wait_for(void) {
  yield_wait_for(DRIVER_NUM_BUZZER, 0);
  return RETURNCODE_SUCCESS;
}
