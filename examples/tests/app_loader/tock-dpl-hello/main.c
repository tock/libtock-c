/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libtock/interface/console.h>

char hello[] = "Tock now supports dynamic app installs!\r\n";

static void nop(
  returncode_t ret __attribute__((unused)),
  uint32_t     bytes_written __attribute__((unused))) {}

int main(void) {
  libtock_console_write((uint8_t*) hello, strlen(hello), nop);
  // Because we used the async method (as opposed to something synchronous,
  // such as printf), we must explicitly wait for the asynchronous write to complete.
  yield();
  // Now we are done.
  return 0;
}
