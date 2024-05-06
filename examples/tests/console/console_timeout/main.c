#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libtock/interface/console.h>
#include <libtock-sync/services/alarm.h>

char buf[100];

alarm_t t;

static void getnstr_cb(returncode_t result __attribute__ ((unused)),
                       uint32_t     len) {
  printf("Userspace call to read console returned: ");
  for (uint32_t i = 0; i < len; i++) {
    printf("%c", buf[i]);
  }
  printf("\n");

  exit(0);
}

static void alarm_cb(__attribute__ ((unused)) uint32_t now,
                     __attribute__ ((unused)) uint32_t scheduled) {
  libtock_console_abort_read();
}

int main(void) {

  int ret = libtock_console_read((uint8_t*) buf, 60, getnstr_cb);
  if (ret != RETURNCODE_SUCCESS) {
    printf("Error doing UART receive.\n");
  }

  // Generate a timeout to abort the receive call.
  libtock_alarm_in_ms(5000, alarm_cb, &t);

  while (1) {
    yield();
  }
}
