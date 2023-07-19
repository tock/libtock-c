#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <console.h>
#include <usb_keyboard_hid.h>
#include <timer.h>
#include <button.h>
#include <led.h>
#include <hmac.h>

int decrypt(const uint8_t*, int, uint8_t*, int);
int hmac(const uint8_t*, int, const uint8_t*, int, uint8_t*, int);


#define HMAC_OUTPUT_BUF_LEN 32
#define NUM_KEYS 4
typedef uint64_t counter_t;

uint8_t hmac_output_buf[HMAC_OUTPUT_BUF_LEN];

char hotp_format_buffer[16];

// For USB keyboard.
uint8_t keyboard_buffer[64];

// Base32-encoded key:
// O775VWOS5TBT6VZ4VNDMB4SOMGKNC2BXOVKCRNDFYJ4WSDLPTELUG24QJCNIT53CDACYE6CDKDQKOXSINABRA5UFOPOU5WIDZJLFBNQ
//
// To generate HOTP values in Python, use:
// >>> import pyotp, hashlib
// >>> otp = pyotp.HOTP("$THE_ABOVE_BASE32_KEY", digest=hashlib.sha256)
// >>> otp.at(1)
// '571577'
const uint8_t key0[64] = {
    0x77, 0xff, 0xda, 0xd9, 0xd2, 0xec, 0xc3, 0x3f,
    0x57, 0x3c, 0xab, 0x46, 0xc0, 0xf2, 0x4e, 0x61,
    0x94, 0xd1, 0x68, 0x37, 0x75, 0x54, 0x28, 0xb4,
    0x65, 0xc2, 0x79, 0x69, 0x0d, 0x6f, 0x99, 0x17,
    0x43, 0x6b, 0x90, 0x48, 0x9a, 0x89, 0xf7, 0x62,
    0x18, 0x05, 0x82, 0x78, 0x43, 0x50, 0xe0, 0xa7,
    0x5e, 0x48, 0x68, 0x03, 0x10, 0x76, 0x85, 0x73,
    0xdd, 0x4e, 0xd9, 0x03, 0xca, 0x56, 0x50, 0xb6
};

counter_t counters[NUM_KEYS] = { 0, 0, 0, 0 };
int key_lengths[NUM_KEYS] = { 64, 5, 3, 3 };
int key_digits[NUM_KEYS] = { 6, 6, 6, 6 };
const uint8_t* keys[NUM_KEYS] = {
  &key0[0],
  (uint8_t*) "world",
  (uint8_t*) "foo",
  (uint8_t*) "bar"
};

static void hmac_upcall(__attribute__ ((unused)) int   arg0,
                        __attribute__ ((unused)) int   arg1,
			__attribute__ ((unused)) int   arg2,
			                         void* done_flag) {
    *((bool *) done_flag) = true;
}

int hmac(const uint8_t* key, int key_len, const uint8_t* data, int data_len, uint8_t* output_buffer, int output_buffer_len) {
    int ret;
    bool hmac_done = false;

    ret = hmac_set_callback(hmac_upcall, &hmac_done);
    if (ret < 0) {
	goto done;
    }

    ret = hmac_set_key_buffer(key, key_len);
    if (ret < 0) {
	goto deregister_upcall;
    }

    ret = hmac_set_dest_buffer(output_buffer, output_buffer_len);
    if (ret < 0) {
	goto unallow_key_buffer;
    }

    ret = hmac_set_data_buffer(data, data_len);
    if (ret < 0) {
	goto unallow_dest_buffer;
    }

    ret = hmac_set_algorithm(TOCK_HMAC_ALG_SHA256);
    if (ret < 0) {
	goto unallow_data_buffer;
    }

    ret = hmac_run();
    if (ret < 0) {
	printf("HMAC failure: %d\r\n", ret);
	goto unallow_data_buffer;
    }

    yield_for(&hmac_done);

 unallow_data_buffer:
    hmac_set_data_buffer(NULL, 0);

 unallow_dest_buffer:
    hmac_set_dest_buffer(NULL, 0);

 unallow_key_buffer:
    hmac_set_key_buffer(NULL, 0);

 deregister_upcall:
    hmac_set_callback(NULL, NULL);

 done:
    return ret;
}

int decrypt(const uint8_t* cipher, int cipherlen, uint8_t* plaintext, int plaintext_capacity) {
  int copylen = plaintext_capacity < cipherlen ? plaintext_capacity : cipherlen;
  memcpy(plaintext, cipher, copylen);
  return copylen;
}

int btn_num;
int btn_val;

// Callback for button presses.
//   btn_num: The index of the button associated with the callback
//   val: 1 if pressed, 0 if depressed
static void button_upcall(int                            num,
                          int                            val,
                          __attribute__ ((unused)) int   arg2,
                          __attribute__ ((unused)) void *ud) {
  btn_num = num;
  btn_val = val;
}


int main(void) {
  int err;

  err = button_subscribe(button_upcall, NULL);
  if (err < 0) return err;

  // Enable interrupts on each button.
  int count;
  err = button_count(&count);
  if (err < 0) return err;

  for (int i = 0; i < count; i++) {
    button_enable_interrupt(i);
  }

  for (;;) {
    btn_num = -1;
    yield();
    if (btn_num == -1) {
      continue;
    }

    if (btn_val == 1 && btn_num < NUM_KEYS && key_lengths[btn_num] > 0) {
      led_toggle(btn_num);

      // Decrypt the key:
      uint8_t key[64];
      int keylen = decrypt(keys[btn_num], key_lengths[btn_num], &key[0], 64);

      // Generate the HMAC'ed data from the "moving factor" (timestamp in TOTP,
      // counter in HOTP), shuffled in a specific way:
      uint8_t moving_factor[sizeof (counter_t)];
      size_t i;
      for (i = 0; i < sizeof (counter_t); i++)
        moving_factor[i] =
    	(counters[btn_num] >> ((sizeof (counter_t) - i - 1) * 8)) & 0xFF;

      // Perform the HMAC operation. TODO! error check, this works exactly once...
      hmac(&key[0], keylen, &moving_factor[0], sizeof (counter_t), hmac_output_buf, HMAC_OUTPUT_BUF_LEN);

      // Finally, increment the counter:
      counters[btn_num]++;

      uint8_t offset = hmac_output_buf[HMAC_OUTPUT_BUF_LEN - 1] & 0x0f;
      uint32_t S = (((hmac_output_buf[offset] & 0x7f) << 24)
    	 | ((hmac_output_buf[offset + 1] & 0xff) << 16)
    	 | ((hmac_output_buf[offset + 2] & 0xff) << 8)
    	 | ((hmac_output_buf[offset + 3] & 0xff)));

      switch (key_digits[btn_num]) {
      case 6:
    	S = S % 1000000;
    	break;
      case 7:
    	S = S % 10000000;
    	break;
      case 8:
    	S = S % 100000000;
    	break;
      default:
    	// TODO: error handling!
    	S = 0;
    	break;
      }

      snprintf(hotp_format_buffer, 16, "%.*ld", key_digits[btn_num], S);

      // Write the value to the USB keyboard.
      //value_buf[value_len] = '\0';
      int ret = usb_keyboard_hid_send_string_sync(keyboard_buffer, &hotp_format_buffer[0], 16);
      if (ret < 0) {
          printf("ERROR sending string with USB keyboard HID: %i\n", ret);
      } else {
          printf("Typed \"%s\" on the USB HID the keyboard\r\n", hotp_format_buffer);
      }
      led_toggle(btn_num);
    } else if (btn_val == 1 && key_lengths[btn_num] == 0) {
        printf("HOTP / TOTP slot %d not yet configured.\r\n", btn_num);
    }
  }

  return 0;
}
