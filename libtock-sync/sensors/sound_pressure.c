#include <sensors/sound_pressure.h>
#include <tock.h>

#include "sound_pressure.h"

struct sound_pressure_data {
  bool fired;
  int sound_pressure;
  returncode_t ret;
};

static struct sound_pressure_data result = { .fired = false };

// Internal callback for faking synchronous reads
static void cb(returncode_t ret, uint8_t sound_pressure) {
  result.sound_pressure = sound_pressure;
  result.fired = true;
  result.ret   = ret;
}


returncode_t libtocksync_sound_pressure_read(uint8_t* sound_pressure) {
  returncode_t err;
  result.fired = false;

  err = libtock_sound_pressure_read(cb);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  *sound_pressure = result.sound_pressure;

  return RETURNCODE_SUCCESS;
}
