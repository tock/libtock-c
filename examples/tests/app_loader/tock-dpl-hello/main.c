/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <console.h>

char hello[] = "Tock now supports dynamic app installs!\r\n";

static void nop(
  int   a __attribute__((unused)),
  int   b __attribute__((unused)),
  int   c __attribute__((unused)),
  void* d __attribute__((unused))) {}

int main(void) {
  putnstr_async(hello, strlen(hello), nop, NULL);
  // Because we used the async method (as opposed to something synchronous,
  // such as printf), we must explicitly wait for the asynchronous write to complete.
  yield();
  // Now we are done.
  return 0;
}
