#include <ctype.h>
#include <stdio.h>

#include <libtock-sync/interface/console.h>
#include <libtock/interface/led.h>

#include "base32.h"
#include "step0.h"
#include "step1.h"

void program_new_secret(hotp_key_t* hotp_key) {
  // Request user input
  libtock_led_on(0);
  printf("Program a new key\r\n");
  printf("(hit enter without typing to cancel)\r\n");

  // Read key values from user
  // TODO: sure would be nice to clear all previous input before starting this
  uint8_t newkey[128];
  int i = 0;
  while (i < 127) {
    // read next character
    char c;
    int number_read, number_written;
    libtocksync_console_read((uint8_t*) &c, 1, &number_read);

    // break on enter
    if (c == '\n') {
      break;
    }

    // only record alphanumeric characters
    if (isalnum((int) c)) {
      newkey[i] = c;
      i++;

      // echo input to user
      libtocksync_console_write((uint8_t*) &c, 1, &number_written);
    }
  }

  // Finished. Append null terminator and echo newline
  newkey[i] = '\0';
  printf("\r\n");

  // Handle early exits
  if (newkey[0] == '\0') {
    printf("Aborted\r\n");
    libtock_led_off(0);
    return;
  }

  // Decode and save secret to flash
  hotp_key->len     = base32_decode(newkey, hotp_key->key, 64);
  hotp_key->counter = 0;

  // Completed!
  printf("Programmed \"%s\" as key\r\n", newkey);
  libtock_led_off(0);
}
