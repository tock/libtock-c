#include "text_screen.h"
#include <stdlib.h>
#include <tock.h>

typedef struct {
  int error;
  int data1;
  int data2;
  bool done;
} TextScreenReturn;

static void text_screen_callback(int error,
                                 int data1,
                                 int data2,
                                 void* ud) {
  TextScreenReturn *ret = (TextScreenReturn*) ud;
  ret->error = error;
  ret->data1 = data1;
  ret->data2 = data2;
  ret->done  = true;
}

static uint8_t *buffer   = NULL;
static size_t buffer_len = 0;

static int text_screen_subscribe (subscribe_cb cb, void *userdata) {
  subscribe_return_t sv = subscribe2(DRIVER_NUM_TEXT_SCREEN, 0, cb, userdata);
  if (sv.success == 0) {
    return tock_error_to_rcode(sv.error);
  }
  return TOCK_SUCCESS;
}

static int text_screen_command (int command_num, int data1, int data2) {
  syscall_return_t res = command2(DRIVER_NUM_TEXT_SCREEN, command_num, data1, data2);
  if (res.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode (res.data[0]);
  }
}

static int text_screen_allow (void* ptr, size_t size) {
  allow_ro_return_t res = allow_readonly(DRIVER_NUM_TEXT_SCREEN, 0, ptr, size);
  if (res.success == 0) {
    return tock_error_to_rcode(res.error);
  }
  return TOCK_SUCCESS;
}

int text_screen_init (size_t len)
{
  int r = TOCK_SUCCESS;
  if (buffer != NULL) {
    r = TOCK_EALREADY;
  } else {
    buffer = (uint8_t*) malloc (len);
    if (buffer != NULL) {
      buffer_len = len;
      r = text_screen_allow (buffer, len);
    } else {
      r = TOCK_FAIL;
    }
  }
  return r;
}

uint8_t* text_screen_buffer (void)
{
  return buffer;
}

int text_screen_display_on (void)
{
  TextScreenReturn ret;
  ret.done  = false;
  ret.error = text_screen_subscribe (text_screen_callback, &ret);
  if (ret.error == TOCK_SUCCESS) {
    ret.error = text_screen_command (2, 0, 0);
    if (ret.error == TOCK_SUCCESS) yield_for(&ret.done);
  }
  return ret.error;
}

int text_screen_display_off (void)
{
  TextScreenReturn ret;
  ret.done  = false;
  ret.error = text_screen_subscribe (text_screen_callback, &ret);
  if (ret.error == TOCK_SUCCESS) {
    ret.error = text_screen_command (3, 0, 0);
    if (ret.error == TOCK_SUCCESS) yield_for(&ret.done);
  }
  return ret.error;
}

int text_screen_blink_on (void)
{
  TextScreenReturn ret;
  ret.done  = false;
  ret.error = text_screen_subscribe (text_screen_callback, &ret);
  if (ret.error == TOCK_SUCCESS) {
    ret.error = text_screen_command (4, 0, 0);
    if (ret.error == TOCK_SUCCESS) yield_for(&ret.done);
  }
  return ret.error;
}

int text_screen_blink_off (void)
{
  TextScreenReturn ret;
  ret.done  = false;
  ret.error = text_screen_subscribe (text_screen_callback, &ret);
  if (ret.error == TOCK_SUCCESS) {
    ret.error = text_screen_command (5, 0, 0);
    if (ret.error == TOCK_SUCCESS) yield_for(&ret.done);
  }
  return ret.error;
}

int text_screen_show_cursor (void)
{
  TextScreenReturn ret;
  ret.done  = false;
  ret.error = text_screen_subscribe (text_screen_callback, &ret);
  if (ret.error == TOCK_SUCCESS) {
    ret.error = text_screen_command (6, 0, 0);
    if (ret.error == TOCK_SUCCESS) yield_for(&ret.done);
  }
  return ret.error;
}

int text_screen_hide_cursor (void)
{
  TextScreenReturn ret;
  ret.done  = false;
  ret.error = text_screen_subscribe (text_screen_callback, &ret);
  if (ret.error == TOCK_SUCCESS) {
    ret.error = text_screen_command (7, 0, 0);
    if (ret.error == TOCK_SUCCESS) yield_for(&ret.done);
  }
  return ret.error;
}

int text_screen_clear (void)
{
  TextScreenReturn ret;
  ret.done  = false;
  ret.error = text_screen_subscribe (text_screen_callback, &ret);
  if (ret.error == TOCK_SUCCESS) {
    ret.error = text_screen_command (9, 0, 0);
    if (ret.error == TOCK_SUCCESS) yield_for(&ret.done);
  }
  return ret.error;
}

int text_screen_home (void)
{
  TextScreenReturn ret;
  ret.done  = false;
  ret.error = text_screen_subscribe (text_screen_callback, &ret);
  if (ret.error == TOCK_SUCCESS) {
    ret.error = text_screen_command (10, 0, 0);
    if (ret.error == TOCK_SUCCESS) yield_for(&ret.done);
  }
  return ret.error;
}

int text_screen_set_cursor (uint8_t col, uint8_t row)
{
  TextScreenReturn ret;
  ret.done  = false;
  ret.error = text_screen_subscribe (text_screen_callback, &ret);
  if (ret.error == TOCK_SUCCESS) {
    ret.error = text_screen_command (11, col, row);
    if (ret.error == TOCK_SUCCESS) yield_for(&ret.done);
  }
  return ret.error;
}

int text_screen_write (size_t len)
{
  TextScreenReturn ret;
  ret.done  = false;
  ret.error = text_screen_subscribe (text_screen_callback, &ret);
  if (ret.error == TOCK_SUCCESS) {
    ret.error = text_screen_command (8, len, 0);
    if (ret.error == TOCK_SUCCESS) yield_for(&ret.done);
  }
  return ret.error;
}

int text_screen_get_size (size_t* width, size_t* height)
{
  TextScreenReturn ret;
  ret.done  = false;
  ret.data1 = 0;
  ret.data2 = 0;
  ret.error = text_screen_subscribe (text_screen_callback, &ret);
  if (ret.error == TOCK_SUCCESS) {
    ret.error = text_screen_command (1, 0, 0);
    if (ret.error == TOCK_SUCCESS) yield_for(&ret.done);
  }
  *width  = ret.data1;
  *height = ret.data2;
  return ret.error;
}