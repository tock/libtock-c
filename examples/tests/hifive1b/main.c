#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libtock/interface/console.h>

char hello[] = "Hello from HiFive1b!\r\n";

static void nop(
  returncode_t a __attribute__((unused)),
  uint32_t     b __attribute__((unused))) {}

int main(void) {
  libtock_console_write((uint8_t*) hello, strlen(hello), nop);

  while (1) {
    yield();
  }
}
