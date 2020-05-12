#include "framebuffer.h"
#include <stdlib.h>
#include <tock.h>

typedef struct {
  int error;
  int data1;
  int data2;
  bool done;
} FrameBufferReturn;

static void framebuffer_callback(int error,
                                 int data1,
                                 int data2,
                                 void* ud) {
  FrameBufferReturn *fbr = (FrameBufferReturn*)ud;
  fbr->error = error;
  fbr->data1 = data1;
  fbr->data2 = data2;
  fbr->done  = true;
}

static uint8_t *buffer   = NULL;
static size_t buffer_len = 0;

static int framebuffer_subscribe (subscribe_cb cb, void *userdata) {
  return subscribe(DRIVER_NUM_FRAMEBUFFER, 0, cb, userdata);
}

static int framebuffer_command (int command_num, int data1, int data2) {
  return command(DRIVER_NUM_FRAMEBUFFER, command_num, data1, data2);
}

static int framebuffer_allow (void* ptr, size_t size) {
  return allow(DRIVER_NUM_FRAMEBUFFER, 0, ptr, size);
}

int framebuffer_get_supported_resolutions (void) {
  FrameBufferReturn fbr;
  fbr.data1 = 0;
  fbr.done  = false;
  framebuffer_subscribe (framebuffer_callback, &fbr);
  fbr.error = framebuffer_command (11, 0, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.data1;
}
int framebuffer_get_supported_resolution (size_t index, size_t *width, size_t *height) {
  FrameBufferReturn fbr;
  fbr.done = false;
  framebuffer_subscribe (framebuffer_callback, &fbr);
  fbr.error = framebuffer_command (12, index, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  *width  = fbr.data1;
  *height = fbr.data2;
  return fbr.error;
}
int framebuffer_get_supported_pixel_formats (void) {
  FrameBufferReturn fbr;
  fbr.data1 = 0;
  fbr.done  = false;
  framebuffer_subscribe (framebuffer_callback, &fbr);
  fbr.error = framebuffer_command (13, 0, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.data1;
}
int framebuffer_get_supported_pixel_format (size_t index) {
  FrameBufferReturn fbr;
  fbr.data1 = SCREEN_PIXEL_FORMAT_NONE;
  fbr.done  = false;
  framebuffer_subscribe (framebuffer_callback, &fbr);
  fbr.error = framebuffer_command (14, index, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.data1;
}

int framebuffer_screen_on (void) {
  FrameBufferReturn fbr;
  fbr.done = false;
  framebuffer_subscribe (framebuffer_callback, &fbr);
  fbr.error = framebuffer_command (2, 0, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.error;
}

int framebuffer_screen_off (void) {
  FrameBufferReturn fbr;
  fbr.done = false;
  framebuffer_subscribe (framebuffer_callback, &fbr);
  fbr.error = framebuffer_command (3, 0, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.error;
}

int framebuffer_invert_on (void) {
  FrameBufferReturn fbr;
  fbr.done = false;
  framebuffer_subscribe (framebuffer_callback, &fbr);
  fbr.error = framebuffer_command (4, 0, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.error;
}

int framebuffer_invert_off (void) {
  FrameBufferReturn fbr;
  fbr.done = false;
  framebuffer_subscribe (framebuffer_callback, &fbr);
  fbr.error = framebuffer_command (5, 0, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.error;
}

int framebuffer_init (size_t len)
{
  int r = TOCK_SUCCESS;
  if (buffer != NULL) {
    r = TOCK_EALREADY;
  }else {
    buffer = (uint8_t*)malloc (len);
    if (buffer != NULL) {
      buffer_len = len;
      r = framebuffer_allow (buffer, len);
    }else {
      r = TOCK_FAIL;
    }
  }
  return r;
}

uint8_t * framebuffer_buffer (void)
{
  return buffer;
}

int framebuffer_get_resolution (size_t *width, size_t *height) {
  FrameBufferReturn fbr;
  fbr.done = false;
  framebuffer_subscribe (framebuffer_callback, &fbr);
  fbr.error = framebuffer_command (23, 0, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  *width  = fbr.data1;
  *height = fbr.data2;
  return fbr.error;
}

int framebuffer_set_resolution (size_t width, size_t height) {
  FrameBufferReturn fbr;
  fbr.done = false;
  framebuffer_subscribe (framebuffer_callback, &fbr);
  fbr.error = framebuffer_command (24, width, height);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.error;
}

int framebuffer_get_bits_per_pixel (size_t format)
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

int framebuffer_get_pixel_format (void) {
  FrameBufferReturn fbr;
  fbr.data1 = SCREEN_PIXEL_FORMAT_NONE;
  fbr.done  = false;
  framebuffer_subscribe (framebuffer_callback, &fbr);
  fbr.error = framebuffer_command (25, 0, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.data1;
}

int framebuffer_set_pixel_format (size_t format) {
  FrameBufferReturn fbr;
  fbr.done = false;
  framebuffer_subscribe (framebuffer_callback, &fbr);
  fbr.error = framebuffer_command (26, format, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.error;
}

int framebuffer_get_rotation (void) {
  FrameBufferReturn fbr;
  fbr.data1 = SCREEN_ROTATION_NORMAL;
  fbr.done  = false;
  framebuffer_subscribe (framebuffer_callback, &fbr);
  fbr.error = framebuffer_command (21, 0, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.data1;
}

int framebuffer_set_rotation (size_t rotation) {
  FrameBufferReturn fbr;
  fbr.done = false;
  framebuffer_subscribe (framebuffer_callback, &fbr);
  fbr.error = framebuffer_command (22, rotation, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.error;
}

int framebuffer_set_color (size_t position, size_t color) {
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

int framebuffer_set_window (uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  return framebuffer_command (100, ((x & 0xFFFF) << 16) | ((y & 0xFFFF)),
                              ((width & 0xFFFF) << 16) | ((height & 0xFFFF)));
}

int framebuffer_fill (size_t color) {
  FrameBufferReturn fbr;
  fbr.done  = false;
  fbr.error = framebuffer_set_color (0, color);
  if (fbr.error == TOCK_SUCCESS) {
    framebuffer_subscribe (framebuffer_callback, &fbr);
    fbr.error = framebuffer_command (300, 0, 0);
    if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  }
  return fbr.error;
}

int framebuffer_write (size_t length) {
  FrameBufferReturn fbr;
  fbr.done = false;
  framebuffer_subscribe (framebuffer_callback, &fbr);
  fbr.error = framebuffer_command (200, length, 0);
  if (fbr.error == TOCK_SUCCESS) yield_for (&fbr.done);
  return fbr.error;
}