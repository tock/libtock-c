#pragma once

////////////////////////////////////////////////////////////////////////////////
//
// All functions included in step 0 that are reused.
//
////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>


// Select how many digits for a key
#define KEY_DIGITS 6

typedef uint64_t counter_t;

typedef struct {
  uint8_t len;
  uint8_t key[64];
  counter_t counter;
} hotp_key_t;

extern bool button_pressed;
extern int pressed_btn_num;

// Configure buttons to wait for presses. If `all` is false, only use the first
// button. If all is `true`, initialize all of the buttons.
int initialize_buttons(bool all);
int hmac(const uint8_t* key, int key_len, const uint8_t* data, int data_len, uint8_t* output_buffer,
                int output_buffer_len);
int decrypt(const uint8_t* cipher, int cipherlen, uint8_t* plaintext, int plaintext_capacity);

void program_default_secret(hotp_key_t* hotp_key);
void get_next_code(hotp_key_t* hotp_key, int key_digits);