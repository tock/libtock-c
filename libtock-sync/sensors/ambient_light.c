#include "ambient_light.h"

#include "syscalls/ambient_light_syscalls.h"

bool libtocksync_ambient_light_exists(void) {
  return libtock_ambient_light_driver_exists();
}

returncode_t libtocksync_ambient_light_read_intensity(int* lux_value) {
  returncode_t err;

  err = libtock_ambient_light_command_start_intensity_reading();
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_ambient_light_yield_wait_for(lux_value);
  return err;
}
