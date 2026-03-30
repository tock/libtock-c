#include <libtock/peripherals/syscalls/usb_syscalls.h>

#include "syscalls/usb_syscalls.h"
#include "usb.h"

bool libtocksync_usb_exists(void) {
  return libtock_usb_driver_exists();
}

returncode_t libtocksync_usb_enable_and_attach(void) {
  returncode_t err;

  err = libtock_usb_command_enable_and_attach();
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtocksync_usb_yield_wait_for();
  return err;
}
