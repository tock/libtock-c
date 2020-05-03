#include <tock.h>
#include <stdlib.h>
#include "framebuffer.h"

static void framebuffer_callback(__attribute__ ((unused)) int value,
                       __attribute__ ((unused)) int unused1,
                       __attribute__ ((unused)) int unused2,
                       void* ud) {
  *(bool*)ud = true;
}

static uint8_t *buffer = NULL;
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

int framebuffer_init (size_t len) 
{
  int r = TOCK_SUCCESS;
  if (buffer != NULL)
  {
    r = TOCK_EALREADY;
  }
  else
  {
    buffer = (uint8_t*)malloc (len);
    if (buffer != NULL) {
      buffer_len = len;
      r = framebuffer_allow (buffer, len);
    }
    else
    {
      r = TOCK_FAIL;
    }
  }
  return r;
}

int framebuffer_set_color (size_t position, size_t color) {
  // TODO color mode
  int r = TOCK_SUCCESS;
  if (position < buffer_len - 2) {
    buffer[position * 2] = (color >> 8) & 0xFF;
    buffer[position * 2+1] = color & 0xFF;
  }
  else
  {
    r = TOCK_ESIZE;
  }
  return r;
}

int framebuffer_set_window (uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  return framebuffer_command (100, ((x & 0xFFFF) << 16) | ((y & 0xFFFF)), ((width & 0xFFFF) << 16) | ((height & 0xFFFF)));
}

int framebuffer_fill (size_t color) {
  framebuffer_set_color (0, color);
  bool done = false;
  framebuffer_subscribe (framebuffer_callback, &done);
  int v = framebuffer_command (300, 0, 0);
  if (v == TOCK_SUCCESS) yield_for (&done);
  return v;
}

int framebuffer_write (size_t length) {
  bool done = false;
  framebuffer_subscribe (framebuffer_callback, &done);
  int v = framebuffer_command (200, length, 0);
  if (v == TOCK_SUCCESS) yield_for (&done);
  return v;
}