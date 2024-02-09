#include <ctype.h>
#include <stdio.h>

#include "tock.h"
#include "usb_keyboard_hid.h"

struct usb_keyboard_hid_data {
  bool fired;
  int callback_type;
};

static struct usb_keyboard_hid_data result = { .fired = false };

// Internal callback for faking synchronous reads
static void usb_keyboard_hid_upcall(int                          callback_type,
                                    __attribute__ ((unused)) int unused1,
                                    __attribute__ ((unused)) int unused2,
                                    void*                        ud) {
  struct usb_keyboard_hid_data* data = (struct usb_keyboard_hid_data*) ud;
  data->callback_type = callback_type;
  data->fired         = true;
}


int usb_keyboard_hid_set_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_USBKEYBOARDHID, 0, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int usb_keyboard_hid_set_receive_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_USBKEYBOARDHID, 1, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

int usb_keyboard_hid_set_send_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_USBKEYBOARDHID, 1, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

int usb_keyboard_hid_send(void) {
  syscall_return_t cval = command(DRIVER_NUM_USBKEYBOARDHID, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
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



static int to_hid_keycode(char c, uint8_t* modifier, uint8_t* key)
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
