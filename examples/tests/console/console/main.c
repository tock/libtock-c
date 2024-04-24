#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libtock-sync/interface/console.h>

static int getch(void) {
  uint8_t buffer[1];
  int number_read;
  libtocksync_console_read(buffer, 1, &number_read);
  return buffer[0];
}

int main(void) {
  // Repeatedly read a character from the console
  // and print a message to report it.
  while (1) {
    int c = getch();

    if (c == RETURNCODE_FAIL) {
      printf("\ngetch() failed!\n");
    } else {
      printf("Got character: '%c'\n", (char) c);
    }
  }
}
