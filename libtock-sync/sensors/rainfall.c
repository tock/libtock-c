#include "rainfall.h"

#include "syscalls/rainfall_syscalls.h"

bool libtocksync_rainfall_exists(void) {
  return libtock_rainfall_driver_exists();
}

returncode_t libtocksync_rainfall_read(uint32_t* rainfall, int hours) {
  returncode_t err;

  err = libtock_rainfall_command_read(hours);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_rainfall_yield_wait_for(rainfall);
  return err;
}
