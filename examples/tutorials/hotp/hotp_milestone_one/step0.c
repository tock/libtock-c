#include <math.h>
#include <stdio.h>
#include <string.h>

#include <libtock-sync/interface/usb_keyboard_hid.h>
#include <libtock/crypto/hmac.h>
#include <libtock/interface/button.h>
#include <libtock/interface/led.h>

#include "base32.h"
#include "step0.h"

// --- Button Handling ---

// Global to keep track of most recently pressed button
int pressed_btn_num;
bool button_pressed = false;

// Callback for button presses.
//   num: The index of the button associated with the callback
//   val: 1 if pressed, 0 if depressed
static void button_callback(__attribute__ ((unused)) returncode_t ret,
                            int                                   num,
                            bool                                  val) {
  if (val) {
    pressed_btn_num = num;
    button_pressed  = true;
  }
}

// Initializes interrupts on a button
int initialize_buttons(bool all) {
  returncode_t err;

  // Determine the number of supported buttons
  int count = 0;
  err = libtock_button_count(&count);
  if (err != RETURNCODE_SUCCESS) {
    return err;
  }

  // Enable interrupts if a button exists
  if (count > 0) {
    if (all) {
      for (int i = 0; i < count; i++) {
        libtock_button_notify_on_press(i, button_callback);
      }
    } else {
      libtock_button_notify_on_press(0, button_callback);
    }
  }

  return RETURNCODE_SUCCESS;
}


// --- HMAC Handling ---

static bool hmac_done = false;
static void hmac_callback(__attribute__ ((unused)) returncode_t ret) {
  hmac_done = true;
}

int hmac(const uint8_t* key, int key_len, const uint8_t* data, int data_len, uint8_t* output_buffer,
         int output_buffer_len) {
  returncode_t ret;
  hmac_done = false;

  ret = libtock_hmac_simple(LIBTOCK_HMAC_SHA256,
                            (uint8_t*) key, key_len,
                            (uint8_t*) data, data_len,
                            output_buffer, output_buffer_len,
                            hmac_callback);
  if (ret != RETURNCODE_SUCCESS) {
    printf("HMAC failure: %d\r\n", ret);
    goto done;
  }

  yield_for(&hmac_done);

done:
  libtock_hmac_set_readonly_allow_data_buffer(NULL, 0);
  libtock_hmac_set_readwrite_allow_destination_buffer(NULL, 0);
  libtock_hmac_set_readonly_allow_key_buffer(NULL, 0);
  libtock_hmac_set_upcall(NULL, NULL);
  return ret;
}

int decrypt(const uint8_t* cipher, int cipherlen, uint8_t* plaintext, int plaintext_capacity) {
  int copylen = cipherlen;
  if (plaintext_capacity < cipherlen) {
    copylen = plaintext_capacity;
  }
  memcpy(plaintext, cipher, copylen);
  return copylen;
}

// --- HOTP ---

void program_default_secret(hotp_key_t* hotp_key) {
  libtock_led_on(0);
  const char* default_secret = "test";

  // Decode base32 to get HOTP key value
  int ret = base32_decode((const uint8_t*)default_secret, hotp_key->key, 64);
  if (ret < 0) {
    printf("ERROR cannot base32 decode secret\r\n");
    hotp_key->len = 0;
    return;
  }

  // Initialize remainder of HOTP key
  hotp_key->len     = ret;
  hotp_key->counter = 0;

  printf("Programmed \"%s\" as key \r\n", default_secret);
  libtock_led_off(0);
}

void get_next_code(hotp_key_t* hotp_key, int key_digits) {
  libtock_led_on(0);

  // Decrypt the key
  uint8_t key[64];
  int keylen = decrypt(hotp_key->key, hotp_key->len, key, 64);

  // Generate the HMAC'ed data from the "moving factor" (timestamp in TOTP,
  // counter in HOTP), shuffled in a specific way:
  uint8_t moving_factor[sizeof(counter_t)];
  for (size_t i = 0; i < sizeof(counter_t); i++) {
    moving_factor[i] = (hotp_key->counter >> ((sizeof(counter_t) - i - 1) * 8)) & 0xFF;
  }

  // Perform the HMAC operation
  const uint8_t HMAC_OUTPUT_BUF_LEN = 32;
  uint8_t hmac_output_buf[HMAC_OUTPUT_BUF_LEN];
  hmac(key, keylen, moving_factor, sizeof(counter_t), hmac_output_buf, HMAC_OUTPUT_BUF_LEN);

  // Increment the counter
  hotp_key->counter++;

  // Get output value
  uint8_t offset = hmac_output_buf[HMAC_OUTPUT_BUF_LEN - 1] & 0x0f;
  uint32_t S     = (((hmac_output_buf[offset] & 0x7f) << 24)
                    | ((hmac_output_buf[offset + 1] & 0xff) << 16)
                    | ((hmac_output_buf[offset + 2] & 0xff) << 8)
                    | ((hmac_output_buf[offset + 3] & 0xff)));

  // Limit output to correct number of digits. Modulus by 10^digits
  double digit_count = pow(10, key_digits);
  S %= (uint32_t)digit_count;

  // Record value as a string
  char hotp_format_buffer[16];
  int len = snprintf(hotp_format_buffer, 16, "%.*ld", key_digits, S);
  if (len < 0) {
    len = 0;
  } else if (len > 16) {
    len = 16;
  }

  // Write the value to the USB keyboard.
  int ret = libtocksync_usb_keyboard_hid_send_string(hotp_format_buffer, len);
  if (ret < 0) {
    printf("ERROR sending string with USB keyboard HID: %i\r\n", ret);
  } else {
    printf("Counter: %u. Typed \"%s\" on the USB HID the keyboard\r\n", (size_t)hotp_key->counter - 1,
           hotp_format_buffer);
  }

  // Complete
  libtock_led_off(0);
}
