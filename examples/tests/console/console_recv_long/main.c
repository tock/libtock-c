#include <stdio.h>

#include <libtock-sync/interface/console.h>

uint8_t buf[100];

int main(void) {
  int number_read;
  int ret = libtocksync_console_read(buf, 61, &number_read);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[SHORT] Error doing UART receive: %i\n", ret);
    return -1;
  }

  printf("\n\nconsole_recv_long: ");
  for (int i = 0; i < number_read; i++) {
    printf("%c", buf[i]);
  }
  printf("\n");

  return 0;
}
