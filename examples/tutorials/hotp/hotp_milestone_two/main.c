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
#include <libtock-sync/services/alarm.h>
#include <libtock-sync/storage/kv.h>
#include <libtock/interface/button.h>
#include <libtock/tock.h>

// Local includes
#include "base32.h"
#include "step0.h"
#include "step1.h"


// --- Definitions for HOTP App ---

// Select how many digits for a key
#define KEY_DIGITS 6

hotp_key_t stored_key = {0};


// --- Persistent Key Handling ---

static int initialize_key(hotp_key_t* hotp_key, int index) {
  int ret;

  uint8_t key[64];
  int key_len = 0;
  uint8_t value[sizeof(hotp_key_t)];
  uint32_t value_len = 0;

  key_len = snprintf((char*) key, 64, "hotp-key-%i", index);

  ret = libtocksync_kv_get(key, key_len, value, sizeof(hotp_key_t), &value_len);

  // Check if we read what looks like a valid key.
  if (ret == RETURNCODE_SUCCESS && value_len == sizeof(hotp_key_t)) {
    // Looks valid
    memcpy(hotp_key, value, sizeof(hotp_key_t));
  }

  return RETURNCODE_SUCCESS;
}

static void save_key(hotp_key_t* hotp_key, int index) {
  int ret;

  // Key is "hotp-key-[index]". Value is the `hotp_key_t` data.
  uint8_t key[64];
  int key_len = 0;
  uint8_t value[sizeof(hotp_key_t)];

  key_len = snprintf((char*) key, 64, "hotp-key-%i", index);

  // Save the key value.
  memcpy(value, hotp_key, sizeof(hotp_key_t));
  ret = libtocksync_kv_set(key, key_len, value, sizeof(hotp_key_t));

  if (ret != 0) {
    printf("ERROR(%i): %s.\r\n", ret, tock_strrcode(ret));
    printf("  Could not store key.\r\n");
  }
}



// --- Main Loop ---

// Performs initialization and interactivity.
int main(void) {
  libtocksync_alarm_delay_ms(1000);
  printf("Tock HOTP App Started. Usage:\r\n"
      "* Press Button 1 to get the next HOTP code for that slot.\r\n"
      "* Hold Button 1 to enter a new HOTP secret for that slot.\r\n");

  // Initialize key from KV store.
  if (initialize_key(&stored_key, 0) != RETURNCODE_SUCCESS) {
    printf("ERROR initializing key from KV\r\n");
    return 1;
  }

  // Initialize buttons
  if (initialize_buttons(false) != RETURNCODE_SUCCESS) {
    printf("ERROR initializing buttons\r\n");
    return 1;
  }

  if (stored_key.len == 0) {
    // Configure a default HOTP secret
    program_default_secret(&stored_key);
  }

  // Main loop. Waits for button presses
  while (true) {
    // Yield until a button is pressed
    button_pressed = false;
    yield_for(&button_pressed);
    int btn_num = pressed_btn_num;

    // Delay and check if button is still pressed, signalling a "hold"
    libtocksync_alarm_delay_ms(500);
    int new_val = 0;
    libtock_button_read(btn_num, &new_val);

    // Handle long presses (program new secret)
    if (new_val) {
      program_new_secret(&stored_key);
      save_key(&stored_key, 0);

      // Handle short presses on already configured keys (output next code)
    } else if (stored_key.len > 0) {
      get_next_code(&stored_key, KEY_DIGITS);
      save_key(&stored_key, 0);

      // Error for short press on a non-configured key
    } else if (stored_key.len == 0) {
      printf("HOTP / TOTP key not yet configured.\r\n");
    }
  }

  return 0;
}
