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
#include <stdio.h>

// Libtock includes
#include <libtock/interface/button.h>
#include <libtock/timer.h>

// Local includes
#include "base32.h"
#include "step0.h"
#include "step1.h"
#include "step2.h"


// --- Definitions for HOTP App ---

#define NUM_KEYS 4

// Select how many digits each key is
// Slot 0: 6 digits
// Slot 1: 6 digits
// Slot 2: 7 digits
// Slot 3: 8 digits
int key_digits[NUM_KEYS] = {6, 6, 7, 8};

// List of HOTP keys.
hotp_key_t stored_keys[NUM_KEYS];


// --- Main Loop ---

// Performs initialization and interactivity.
int main(void) {
  delay_ms(1000);
  printf("Tock HOTP App Started. Usage:\r\n"
      "* Press a button to get the next HOTP code for that slot.\r\n"
      "* Hold a button to enter a new HOTP secret for that slot.\r\n");

  // Initialize keys from KV store.
  for (int i = 0; i < NUM_KEYS; i++) {
    if (initialize_key(&stored_keys[i], i) != RETURNCODE_SUCCESS) {
      printf("ERROR initializing key from KV\r\n");
      return 1;
    }
  }

  // Initialize buttons
  if (initialize_buttons() != RETURNCODE_SUCCESS) {
    printf("ERROR initializing buttons\r\n");
    return 1;
  }

  // Configure a default HOTP secret
  program_default_secret(&stored_keys[0]);

  // Main loop. Waits for button presses
  while (true) {
    // Yield until a button is pressed
    yield_for(&button_pressed);
    int btn_num = pressed_btn_num;

    // Delay and check if button is still pressed, signalling a "hold"
    delay_ms(500);
    int new_val = 0;
    libtock_button_read(btn_num, &new_val);

    // Handle long presses (program new secret)
    if (new_val) {
      program_new_secret(&stored_keys[btn_num]);
      save_key(&stored_keys[btn_num], btn_num);

    } else if (btn_num < NUM_KEYS && stored_keys[btn_num].len > 0) {
      // Handle short presses on already configured keys (output next code).
      get_next_code(&stored_keys[btn_num], key_digits[btn_num]);
      save_key(&stored_keys[btn_num], btn_num);

    } else if (stored_keys[btn_num].len == 0) {
      // Error for short press on a non-configured key.
      printf("HOTP / TOTP slot %d not yet configured.\r\n", btn_num);
    }
  }

  return 0;
}
