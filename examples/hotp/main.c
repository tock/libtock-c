#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <console.h>
#include <usb_keyboard_hid.h>
#include <timer.h>
#include <button.h>
#include <led.h>

int decrypt(const char*, int, char*, int);
int hmac(const char*, int, int, uint8_t*, int);


#define VAL_LEN 64
#define NUM_KEYS 4

// For K-V store.
uint8_t value_buf[VAL_LEN];

// For USB keyboard.
uint8_t keyboard_buffer[64];

int counters[4] = { 0, 0, 0, 0};
const char* keys[4] = {
  "hello",
  "world",
  "foo",
  "bar"
};

int hmac(const char* key, int keylen, int counter, uint8_t* output_buffer, int output_buffer_len) {
  return snprintf((char *) output_buffer, output_buffer_len, "%d: %s", counter, key);
}

int decrypt(const char* cipher, int cipherlen, char* plaintext, int plaintext_capacity) {
  int copylen = plaintext_capacity < cipherlen ? plaintext_capacity : cipherlen;
  memcpy(plaintext, cipher, copylen);
  return copylen;
}

// Callback for button presses.
//   btn_num: The index of the button associated with the callback
//   val: 1 if pressed, 0 if depressed
static void button_callback(int                            btn_num,
                            int                            val,
                            __attribute__ ((unused)) int   arg2,
                            __attribute__ ((unused)) void *ud) {
  if (val == 1 && btn_num < NUM_KEYS) {
    led_toggle(btn_num);

    char key[64];
    int keylen = decrypt(keys[btn_num], strlen(keys[btn_num]), key, 64);

    // Retrieve the value.
    int value_len = hmac(key, keylen, counters[btn_num], value_buf, VAL_LEN);
    counters[btn_num]++;

    // Write the value to the USB keyboard.
    //value_buf[value_len] = '\0';
    int ret = usb_keyboard_hid_send_string_sync(keyboard_buffer, (char*)value_buf, value_len);
    if (ret < 0) {
        printf("ERROR sending string with USB keyboard HID: %i\n", ret);
    } else {
        printf("Typed \"%s\" on the USB HID the keyboard\r\n", value_buf);
    }
    led_toggle(btn_num);
  }
}


int main(void) {

  int err;

  err = button_subscribe(button_callback, NULL);
  if (err < 0) return err;

  // Enable interrupts on each button.
  int count;
  err = button_count(&count);
  if (err < 0) return err;

  for (int i = 0; i < count; i++) {
    button_enable_interrupt(i);
  }

  return 0;
}
