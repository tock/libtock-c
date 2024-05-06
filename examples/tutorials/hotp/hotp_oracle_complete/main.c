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
#include <string.h>

// Libtock includes
#include <libtock-sync/interface/console.h>
#include <libtock-sync/interface/usb_keyboard_hid.h>
#include <libtock-sync/storage/kv.h>
#include <libtock/interface/button.h>
#include <libtock/interface/led.h>
#include <libtock-sync/services/alarm.h>

// Local includes
#include "base32.h"
#include "oracle.h"
#include "step0.h"


// --- Definitions for HOTP App ---

#define NUM_KEYS 4

// Select how many digits each key is
// Slot 0: 6 digits
// Slot 1: 6 digits
// Slot 2: 7 digits
// Slot 3: 8 digits
int key_digits[NUM_KEYS] = {6, 6, 7, 8};

typedef struct __attribute__((__packed__)) {
  uint8_t len;
  uint8_t iv[16];
  uint8_t key[64];
  counter_t counter;
} hotp_encrypted_key_t;

// List of HOTP keys.
hotp_encrypted_key_t keys[NUM_KEYS];



// --- Persistent Key Handling ---

static void program_default_encrypted_secret(void);

static void save_key(int slot_num) {
  int ret;
  uint8_t key[64];
  uint8_t value[sizeof(hotp_key_t)];

  // Key is "hotp-key-<slot_num>". Value is the `hotp_key_t` data.
  int key_len = snprintf((char*) key, 64, "hotp-key-%i", slot_num);
  memcpy(value, &keys[slot_num], sizeof(hotp_key_t));
  ret = libtocksync_kv_set(key, key_len, value, sizeof(hotp_key_t));

  if (ret != 0) {
    printf("ERROR(%i): %s.\r\n", ret, tock_strrcode(ret));
    printf("  Could not store key.\r\n");
  }
}

static int initialize_keys(void) {
  int ret;

  // Recover keys from key value store if they exist.
  for (int i = 0; i < NUM_KEYS; i++) {
    uint8_t key[64];
    uint8_t value[sizeof(hotp_key_t)];

    // Try to read the key.
    int key_len = snprintf((char*) key, 64, "hotp-key-%i", i);

    uint32_t value_len = 0;
    ret = libtocksync_kv_get(key, key_len, value, sizeof(hotp_key_t), &value_len);

    // Check if we read what looks like a valid key.
    if (ret != RETURNCODE_SUCCESS || value_len != sizeof(hotp_key_t)) {
      keys[i].len = 0;
      save_key(i);

      if (i == 0) {
        program_default_encrypted_secret();
      }
    } else {
      // Looks valid, copy into our local array of keys.
      memcpy(&keys[i], value, sizeof(hotp_key_t));
    }

  }

  return RETURNCODE_SUCCESS;
}

// --- HOTP Actions ---

static void program_secret(int slot_num, uint8_t* secret) {
  uint8_t plaintext_key[64];
  // Decode base32 to get HOTP key value
  int ret = base32_decode((const uint8_t*)secret, plaintext_key, 64);
  if (ret < 0 ) {
    printf("ERROR cannot base32 decode secret\r\n");
    keys[slot_num].len = 0;
    return;
  }

  ret = oracle_encrypt(plaintext_key, ret, keys[slot_num].key, 64, keys[slot_num].iv);
  if (ret < 0 ) {
    printf("ERROR(%i): %s.\r\n", ret, tock_strrcode(ret));
    printf("ERROR cannot encrypt key\r\n");
    keys[slot_num].len = 0;
    return;
  }

  // Initialize remainder of HOTP key
  keys[slot_num].len     = ret;
  keys[slot_num].counter = 0;
  save_key(slot_num);

  // Completed!
  printf("Programmed \"%s\" to slot %d\r\n", secret, slot_num);
}

static void program_default_encrypted_secret(void) {
  libtock_led_on(0);
  const char* default_secret = "test";
  program_secret(0, (uint8_t*) default_secret);
  libtock_led_off(0);
}

static void program_new_secret(int slot_num) {
  // Request user input
  libtock_led_on(slot_num);
  printf("Program a new key in slot %d\r\n", slot_num);
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

  program_secret(slot_num, newkey);
  libtock_led_off(slot_num);
}

static void get_next_code_encrypted(int slot_num) {
  libtock_led_on(slot_num);

  // Decrypt the key
  uint8_t key[64];
  int keylen = oracle_decrypt(keys[slot_num].iv, keys[slot_num].key, keys[slot_num].len, key, 64);

  // Generate the HMAC'ed data from the "moving factor" (timestamp in TOTP,
  // counter in HOTP), shuffled in a specific way:
  uint8_t moving_factor[sizeof(counter_t)];
  for (size_t i = 0; i < sizeof(counter_t); i++) {
    moving_factor[i] = (keys[slot_num].counter >> ((sizeof(counter_t) - i - 1) * 8)) & 0xFF;
  }

  // Perform the HMAC operation
  const uint8_t HMAC_OUTPUT_BUF_LEN = 32;
  uint8_t hmac_output_buf[HMAC_OUTPUT_BUF_LEN];
  hmac(key, keylen, moving_factor, sizeof(counter_t), hmac_output_buf, HMAC_OUTPUT_BUF_LEN);

  // Increment the counter and save to flash
  keys[slot_num].counter++;
  save_key(slot_num);

  // Get output value
  uint8_t offset = hmac_output_buf[HMAC_OUTPUT_BUF_LEN - 1] & 0x0f;
  uint32_t S     = (((hmac_output_buf[offset] & 0x7f) << 24)
                    | ((hmac_output_buf[offset + 1] & 0xff) << 16)
                    | ((hmac_output_buf[offset + 2] & 0xff) << 8)
                    | ((hmac_output_buf[offset + 3] & 0xff)));

  // Limit output to correct number of digits. Modulus by 10^digits
  double digit_count = pow(10, key_digits[slot_num]);
  S %= (uint32_t)digit_count;

  // Record value as a string
  char hotp_format_buffer[16];
  int len = snprintf(hotp_format_buffer, 16, "%.*ld", key_digits[slot_num], S);
  if (len < 0) {
    len = 0;
  } else if (len > 16) {
    len = 16;
  }

  if (libtock_usb_keyboard_hid_exists()) {
    // Write the value to the USB keyboard.
    int ret = libtocksync_usb_keyboard_hid_send_string(hotp_format_buffer, len);
    if (ret < 0) {
      printf("ERROR sending string with USB keyboard HID: %i\r\n", ret);
    } else {
      printf("Counter: %u. Typed \"%s\" on the USB HID the keyboard\r\n", (size_t)keys[slot_num].counter - 1,
             hotp_format_buffer);
    }
  } else {
    // Fallback to just print on the console.
    hotp_format_buffer[len] = '\0';
    printf("CODE: %s\n", hotp_format_buffer);
  }

  // Complete
  libtock_led_off(slot_num);
}


// --- Main Loop ---

// Performs initialization and interactivity.
int main(void) {
  printf("Tock HOTP App Started. Usage:\r\n"
      "* Press a button to get the next HOTP code for that slot.\r\n"
      "* Hold a button to enter a new HOTP secret for that slot.\r\n");

  // Initialize keys from KV store.
  if (initialize_keys() != RETURNCODE_SUCCESS) {
    printf("ERROR initializing app store\r\n");
    return 1;
  }

  // Initialize buttons
  if (initialize_buttons() != RETURNCODE_SUCCESS) {
    printf("ERROR initializing buttons\r\n");
    return 1;
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
      program_new_secret(btn_num);

      // Handle short presses on already configured keys (output next code)
    } else if (btn_num < NUM_KEYS && keys[btn_num].len > 0) {
      get_next_code_encrypted(btn_num);

      // Error for short press on a non-configured key
    } else if (keys[btn_num].len == 0) {
      printf("HOTP / TOTP slot %d not yet configured.\r\n", btn_num);
    }
  }

  return 0;
}
