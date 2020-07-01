#include "screen.h"
#include <stdlib.h>
#include <tock.h>

typedef struct {
  int error;
  int data1;
  int data2;
  bool done;
} ScreenReturn;

static void screen_callback(int error,
                            int data1,
                            int data2,
                            void* ud) {
  ScreenReturn *fbr = (ScreenReturn*)ud;
  fbr->error = error;
  fbr->data1 = data1;
  fbr->data2 = data2;
  fbr->done  = true;
}

static uint8_t *buffer   = NULL;
static size_t buffer_len = 0;

static int screen_subscribe (subscribe_cb cb, void *userdata) {
  return subscribe(DRIVER_NUM_SCREEN, 0, cb, userdata);
}

static int screen_command (int command_num, int data1, int data2) {
  return command(DRIVER_NUM_SCREEN, command_num, data1, data2);
}

static int screen_allow (void* ptr, size_t size) {
  return allow(DRIVER_NUM_SCREEN, 0, ptr, size);
}

int screen_get_supported_resolutions (void) {
  ScreenReturn fbr;
  fbr.data1 = 0;
  fbr.done  = false;
  screen_subscribe (screen_callback, &fbr);
  fbr.error = screen_command (11, 0, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.data1;
}
int screen_get_supported_resolution (size_t index, size_t *width, size_t *height) {
  ScreenReturn fbr;
  fbr.done = false;
  screen_subscribe (screen_callback, &fbr);
  fbr.error = screen_command (12, index, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  *width  = fbr.data1;
  *height = fbr.data2;
  return fbr.error;
}
int screen_get_supported_pixel_formats (void) {
  ScreenReturn fbr;
  fbr.data1 = 0;
  fbr.done  = false;
  screen_subscribe (screen_callback, &fbr);
  fbr.error = screen_command (13, 0, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.data1;
}
int screen_get_supported_pixel_format (size_t index) {
  ScreenReturn fbr;
  fbr.data1 = SCREEN_PIXEL_FORMAT_ERROR;
  fbr.done  = false;
  screen_subscribe (screen_callback, &fbr);
  fbr.error = screen_command (14, index, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.data1;
}

bool screen_setup_enabled (void) {
  return screen_command (1, 0, 0) != 0;
}

int screen_set_brightness (size_t brightness) {
  ScreenReturn fbr;
  fbr.done = false;
  screen_subscribe (screen_callback, &fbr);
  fbr.error = screen_command (3, brightness, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.error;
}

int screen_invert_on (void) {
  ScreenReturn fbr;
  fbr.done = false;
  screen_subscribe (screen_callback, &fbr);
  fbr.error = screen_command (4, 0, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.error;
}

int screen_invert_off (void) {
  ScreenReturn fbr;
  fbr.done = false;
  screen_subscribe (screen_callback, &fbr);
  fbr.error = screen_command (5, 0, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.error;
}

int screen_init (size_t len)
{
  int r = TOCK_SUCCESS;
  if (buffer != NULL) {
    r = TOCK_EALREADY;
  }else {
    buffer = (uint8_t*)malloc (len);
    if (buffer != NULL) {
      buffer_len = len;
      r = screen_allow (buffer, len);
    }else {
      r = TOCK_FAIL;
    }
  }
  return r;
}

uint8_t * screen_buffer (void)
{
  return buffer;
}

int screen_get_resolution (size_t *width, size_t *height) {
  ScreenReturn fbr;
  fbr.done = false;
  screen_subscribe (screen_callback, &fbr);
  fbr.error = screen_command (23, 0, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  *width  = fbr.data1;
  *height = fbr.data2;
  return fbr.error;
}

int screen_set_resolution (size_t width, size_t height) {
  ScreenReturn fbr;
  fbr.done = false;
  screen_subscribe (screen_callback, &fbr);
  fbr.error = screen_command (24, width, height);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.error;
}

int screen_get_bits_per_pixel (size_t format)
{
  switch (format) {
    case SCREEN_PIXEL_FORMAT_MONO:
      return 1;

    case SCREEN_PIXEL_FORMAT_RGB_233:
      return 8;

    case SCREEN_PIXEL_FORMAT_RGB_565:
      return 16;

    case SCREEN_PIXEL_FORMAT_RGB_888:
      return 24;

    case SCREEN_PIXEL_FORMAT_ARGB_8888:
      return 32;

    default:
      return 0;
  }
}

int screen_get_pixel_format (void) {
  ScreenReturn fbr;
  fbr.data1 = SCREEN_PIXEL_FORMAT_ERROR;
  fbr.done  = false;
  screen_subscribe (screen_callback, &fbr);
  fbr.error = screen_command (25, 0, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.data1;
}

int screen_set_pixel_format (size_t format) {
  ScreenReturn fbr;
  fbr.done = false;
  screen_subscribe (screen_callback, &fbr);
  fbr.error = screen_command (26, format, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.error;
}

int screen_get_rotation (void) {
  ScreenReturn fbr;
  fbr.data1 = SCREEN_ROTATION_NORMAL;
  fbr.done  = false;
  screen_subscribe (screen_callback, &fbr);
  fbr.error = screen_command (21, 0, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.data1;
}

int screen_set_rotation (size_t rotation) {
  ScreenReturn fbr;
  fbr.done = false;
  screen_subscribe (screen_callback, &fbr);
  fbr.error = screen_command (22, rotation, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.error;
}

int screen_set_color (size_t position, size_t color) {
  // TODO color mode
  int r = TOCK_SUCCESS;
  if (position < buffer_len - 2) {
    buffer[position * 2]     = (color >> 8) & 0xFF;
    buffer[position * 2 + 1] = color & 0xFF;
  }else {
    r = TOCK_ESIZE;
  }
  return r;
}

int screen_set_frame (uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  ScreenReturn fbr;
  fbr.done = false;
  screen_subscribe (screen_callback, &fbr);
  fbr.error = screen_command (100, ((x & 0xFFFF) << 16) | ((y & 0xFFFF)),
                              ((width & 0xFFFF) << 16) | ((height & 0xFFFF)));
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.error;
}

int screen_fill (size_t color) {
  ScreenReturn fbr;
  fbr.done  = false;
  fbr.error = screen_set_color (0, color);
  if (fbr.error == TOCK_SUCCESS) {
    screen_subscribe (screen_callback, &fbr);
    fbr.error = screen_command (300, 0, 0);
    if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  }
  return fbr.error;
}

int screen_write (size_t length) {
  ScreenReturn fbr;
  fbr.done = false;
  screen_subscribe (screen_callback, &fbr);
  fbr.error = screen_command (200, length, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.error;
}