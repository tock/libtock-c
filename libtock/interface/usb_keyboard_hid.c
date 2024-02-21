#include <ctype.h>
#include <stdio.h>

#include "tock.h"
#include "usb_keyboard_hid.h"



static void usb_keyboard_hid_upcall(int                          callback_type,
                                    __attribute__ ((unused)) int unused1,
                                    __attribute__ ((unused)) int unused2,
                                    void*                        opaque) {
  usb_keyboard_hid_callback cb = (usb_keyboard_hid_callback) opaque;
  cb();
}




returncode_t libtock_usb_keyboard_hid_send(uint8_t* buffer, uint32_t len, usb_keyboard_hid_callback cb) {
  returncode_t err;
  result.fired = false;

  err = libtock_usb_keyboard_hid_set_upcall(usb_keyboard_hid_upcall, (void*) cb);
  if (err < 0) return err;

  err = libtock_usb_keyboard_hid_set_readwrite_allow_send_buffer(buffer, len);
  if (err < 0) return err;

  err = libtock_usb_keyboard_hid_command_send(); // Sometimes returns ERESERVE (but everything keeps working??)
  return err;
}


int usb_keyboard_hid_send_sync(uint8_t* buffer, uint32_t len) {
  int err;
  result.fired = false;

  err = usb_keyboard_hid_set_callback(usb_keyboard_hid_upcall, (void*) &result);
  if (err < 0) return err;

  err = usb_keyboard_hid_set_send_buffer(buffer, len);
  if (err < 0) return err;

  err = usb_keyboard_hid_send(); // Sometimes returns ERESERVE (but everything keeps working??)
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  err = usb_keyboard_hid_set_send_buffer(NULL, 0);
  if (err < 0) return err;

  return RETURNCODE_SUCCESS;
}



static int libtock_to_hid_keycode(char c, uint8_t* modifier, uint8_t* key)
{
  uint8_t shift = 2;  // KB_MODIFIER_LEFT_SHIFT = 2

  // Clear modifier.
  *modifier = 0;

  if ((c >= 'A') && (c <= 'Z')) {
    c         = tolower((int) c);
    *modifier = shift;
  }
  if ((c >= 'a') && (c <= 'z')) {
    *key = ((c -= 'a') + 4);
    return 0;
  }
  if ((c >= '1') && (c <= '9')) {
    *key = ((c -= '0') + 0x1D);
    return 0;
  }
  switch (c) {
    case '!':   *modifier = shift;
      *key = 0x1E;
      return 0;
    case '@':   *modifier = shift;
      *key = 0x1F;
      return 0;
    case '#':   *modifier = shift;
      *key = 0x20;
      return 0;
    case '$':   *modifier = shift;
      *key = 0x21;
      return 0;
    case '%':   *modifier = shift;
      *key = 0x22;
      return 0;
    case '^':   *modifier = shift;
      *key = 0x23;
      return 0;
    case '&':   *modifier = shift;
      *key = 0x24;
      return 0;
    case '*':   *modifier = shift;
      *key = 0x25;
      return 0;
    case '(':   *modifier = shift;
      *key = 0x26;
      return 0;
    case ')':   *modifier = shift;
      *key = 0x27;
      return 0;
    case '0':   *key = 0x27;
      return 0;
    case '\n':  *key = 0x28;
      return 0;                          // enter
    case '\r':  *key = 0x28;
      return 0;                          // enter
    case '\b':  *key = 0x2A;
      return 0;                          // backspace
    case '\t':  *key = 0x2B;
      return 0;                          // tab
    case ' ':   *key = 0x2C;
      return 0;                          // space
    case '_':   *modifier = shift;
      *key = 0x2D;
      return 0;
    case '-':   *key = 0x2D;
      return 0;
    case '+':   *modifier = shift;
      *key = 0x2E;
      return 0;
    case '=':   *key = 0x2E;
      return 0;
    case '{':   *modifier = shift;
      *key = 0x2F;
      return 0;
    case '[':   *key = 0x2F;
      return 0;
    case '}':   *modifier = shift;
      *key = 0x30;
      return 0;
    case ']':   *key = 0x30;
      return 0;
    case '|':   *modifier = shift;
      *key = 0x31;
      return 0;
    case '\\':   *key = 0x31;
      return 0;
    case ':':   *modifier = shift;
      *key = 0x33;
      return 0;
    case ';':   *key = 0x33;
      return 0;
    case '"':   *modifier = shift;
      *key = 0x34;
      return 0;
    case '\'':   *key = 0x34;
      return 0;
    case '~':   *modifier = shift;
      *key = 0x35;
      return 0;
    case '`':   *key = 0x35;
      return 0;
    case '<':   *modifier = shift;
      *key = 0x36;
      return 0;
    case ',':   *key = 0x36;
      return 0;
    case '>':   *modifier = shift;
      *key = 0x37;
      return 0;
    case '.':   *key = 0x37;
      return 0;
    case '?':   *modifier = shift;
      *key = 0x38;
      return 0;
    case '/':   *key = 0x38;
      return 0;
  }
  return -1;
}



int usb_keyboard_hid_send_letter_sync(char letter) {

  uint8_t modifier;
  uint8_t key = 0;
  to_hid_keycode(letter, &modifier, &key);

  uint8_t buffer[64];
  buffer[0] = modifier;
  buffer[1] = 0;
  buffer[2] = key;
  buffer[3] = 0;
  buffer[4] = 0;
  buffer[5] = 0;
  buffer[6] = 0;
  buffer[7] = 0;

  // Send key press.
  int err = usb_keyboard_hid_send_sync(buffer, 64);
  if (err < 0) return err;

  buffer[0] = 0;
  buffer[2] = 0;

  // Clear key press.
  err = usb_keyboard_hid_send_sync(buffer, 64);
  if (err < 0) return err;

  return RETURNCODE_SUCCESS;
}

int usb_keyboard_hid_send_string_sync(char* str, int length) {
  for (int i = 0; i < length; i++) {
    int err = usb_keyboard_hid_send_letter_sync(str[i]);
    if (err < 0) return err;
  }

  return RETURNCODE_SUCCESS;
}
