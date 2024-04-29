#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libtock/interface/console.h>
#include <libtock/timer.h>

char buf[100];

tock_timer_t t;

static void getnstr_cb(returncode_t result __attribute__ ((unused)),
                       uint32_t     len) {
  printf("Userspace call to read console returned: ");
  for (uint32_t i = 0; i < len; i++) {
    printf("%c", buf[i]);
  }
  printf("\n");

  exit(0);
}

static void timer_cb(int   result __attribute__ ((unused)),
                     int   _y __attribute__ ((unused)),
                     int   _z __attribute__ ((unused)),
                     void* ud __attribute__ ((unused))) {
  libtock_console_abort_read();
}

int main(void) {

  int ret = libtock_console_read((uint8_t*) buf, 60, getnstr_cb);
  if (ret != RETURNCODE_SUCCESS) {
    printf("Error doing UART receive.\n");
  }

  // Generate a timeout to abort the receive call.
  timer_in(5000, timer_cb, NULL, &t);

  while (1) {
    yield();
  }
}
