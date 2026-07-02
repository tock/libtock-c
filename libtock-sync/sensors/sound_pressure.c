#include "sound_pressure.h"

#include "syscalls/sound_pressure_syscalls.h"

bool libtocksync_sound_pressure_exists(void) {
  return libtock_sound_pressure_driver_exists();
}

returncode_t libtocksync_sound_pressure_enable(void) {
  return libtock_sound_pressure_command_enable();
}

returncode_t libtocksync_sound_pressure_disable(void) {
  return libtock_sound_pressure_command_disable();
}

returncode_t libtocksync_sound_pressure_read(uint8_t* sound_pressure) {
  returncode_t err;

  err = libtock_sound_pressure_command_read();
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_sound_pressure_yield_wait_for(sound_pressure);
  return err;
}
