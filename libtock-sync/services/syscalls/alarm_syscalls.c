#include "alarm_syscalls.h"

returncode_t libtocksync_alarm_yield_wait_for(void) {
  yield_wait_for(DRIVER_NUM_ALARM, 0);
  return RETURNCODE_SUCCESS;
}
