// USB Security Key Application
//
// Outputs HOTP codes over USB HID
//
// Test with: https://www.verifyr.com/en/otp/check#hotp
// Use the "Generate HOTP Code" window with the default secret "test" or whatever secret you use
// Counter should be the current counter value
// MUST use algorithm "sha256"
// Digits should be "6" unless you later change that

// C standard library includes
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Libtock includes
#include <libtock-sync/interface/console.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/interface/button.h>
#include <libtock/interface/led.h>

// Local includes
#include "base32.h"
#include "step0.h"


// --- Definitions for HOTP App ---

// Select how many digits for a key
#define KEY_DIGITS 6

hotp_key_t stored_key = {0};


// --- HOTP Actions ---

static void program_new_secret(hotp_key_t* hotp_key) {
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


// --- Main Loop ---

// Performs initialization and interactivity.
int main(void) {
  libtocksync_alarm_delay_ms(1000);
  printf("Tock HOTP App Started. Usage:\r\n"
      "* Press Button 1 to get the next HOTP code for that slot.\r\n"
      "* Hold Button 1 to enter a new HOTP secret for that slot.\r\n");

  // Initialize buttons
  if (initialize_buttons() != RETURNCODE_SUCCESS) {
    printf("ERROR initializing buttons\r\n");
    return 1;
  }

  // Configure a default HOTP secret
  program_default_secret(&stored_key);

  // Main loop. Waits for button presses
  while (true) {
    // Yield until a button is pressed
    yield_for(&button_pressed);
    int btn_num = pressed_btn_num;

    // Delay and check if button is still pressed, signalling a "hold"
    libtocksync_alarm_delay_ms(500);
    int new_val = 0;
    libtock_button_read(btn_num, &new_val);

    // Handle long presses (program new secret)
    if (new_val) {
      program_new_secret(&stored_key);

      // Handle short presses on already configured keys (output next code)
    } else if (stored_key.len > 0) {
      get_next_code(&stored_key, KEY_DIGITS);

      // Error for short press on a non-configured key
    } else if (stored_key.len == 0) {
      printf("HOTP / TOTP key not yet configured.\r\n");
    }
  }

  return 0;
}
