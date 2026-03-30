#include "sound_pressure_syscalls.h"

returncode_t libtocksync_sound_pressure_yield_wait_for(uint8_t* sound_pressure) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_SOUND_PRESSURE, 0);
  *sound_pressure = (uint8_t) ret.data0;
  return RETURNCODE_SUCCESS;
}
