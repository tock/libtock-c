#include "eui64.h"
#include "syscalls/eui64_syscalls.h"

bool libtock_eui64_exists(void) {
  return libtock_eui64_driver_exists();
}

returncode_t libtock_eui64_get(uint64_t* eui64) {
  return libtock_eui64_command_get(eui64);
}
