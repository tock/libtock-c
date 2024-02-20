#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "console.h"

int putnstr(const char *str, size_t len) {
  int ret = RETURNCODE_SUCCESS;

  allow_ro_return_t ro = allow_readonly(DRIVER_NUM_CONSOLE, 1, str, len);
  if (!ro.success) {
    return tock_status_to_returncode(ro.status);
  }

  syscall_return_t com = command(DRIVER_NUM_CONSOLE, 1, len, 0);
  ret = tock_command_return_novalue_to_returncode(com);
  if (ret < 0) {
    return ret;
  }

  return yield_for_subscribable_upcall_returnr0_sync(DRIVER_NUM_CONSOLE, 2);
}


int getnstr(char *buf, size_t len) {
  int ret;

  allow_rw_return_t rw = allow_readwrite(DRIVER_NUM_CONSOLE, 1, buf, len);
  if (!rw.success) {
    return tock_status_to_returncode(rw.status);
  }

  syscall_return_t com = command(DRIVER_NUM_CONSOLE, 2, len, 0);
  ret = tock_command_return_novalue_to_returncode(com);
  if (ret < 0) {
    return ret;
  }

  return yield_for_subscribable_upcall_returnr0_sync(DRIVER_NUM_CONSOLE, 2);
}

int getch(void) {
  int r;
  char buf[1];

  r = getnstr(buf, 1);
  return (r == RETURNCODE_SUCCESS) ? buf[0] : RETURNCODE_FAIL;
}
