#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "console.h"

static void generic_upcall(int   status,
                           int   length,
                           int   _z __attribute__ ((unused)),
                           void* ud) {
  libtock_console_callback_write cb = (libtock_console_callback_write)ud;
  cb(tock_status_to_returncode(status), length);
}

returncode_t libtock_console_write(const uint8_t* buffer, uint32_t len, libtock_console_callback_write cb) {
  returncode_t err;

  err = libtock_console_write_done_set_upcall(generic_upcall, (void*)cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_console_set_read_allow(buffer, len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_console_command_write(len);
  return err;
}

returncode_t libtock_console_read(uint8_t* buffer, uint32_t len, libtock_console_callback_read cb) {
  returncode_t err;

  err = libtock_console_read_done_set_upcall(generic_upcall, (void*)cb);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_console_set_readwrite_allow(buffer, len);
  if (err != RETURNCODE_SUCCESS) return err;

  err = libtock_console_command_read(len);
  return err;
}

returncode_t libtock_console_abort_read(void) {
  return libtock_console_command_abort_read();
}
