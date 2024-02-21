#include <stdlib.h>

#include "screen.h"
#include "syscalls/screen_syscalls.h"



static void screen_callback_done(int                          status,
                                 __attribute__ ((unused)) int data1,
                                 __attribute__ ((unused)) int data2,
                                 void*                        opaque) {
  libtock_screen_callback_done cb = (libtock_screen_callback_done) opaque;
  cb(tock_status_to_returncode(status));
}

static void screen_callback_format(int                          status,
                                   __attribute__ ((unused)) int data1,
                                   __attribute__ ((unused)) int data2,
                                   void*                        opaque) {
  libtock_screen_callback_format cb = (libtock_screen_callback_format) opaque;
  cb(tock_status_to_returncode(status), (libtock_screen_format_t) data1);
}

static void screen_callback_rotation(int                          status,
                                     __attribute__ ((unused)) int data1,
                                     __attribute__ ((unused)) int data2,
                                     void*                        opaque) {
  libtock_screen_callback_rotation cb = (libtock_screen_callback_rotation) opaque;
  cb(tock_status_to_returncode(status), (libtock_screen_rotation_t) data1);
}


statuscode_t libtock_screen_buffer_init(size_t len, uint8_t** buffer) {
  if (*buffer != NULL) return TOCK_STATUSCODE_ALREADY;

  *buffer = (uint8_t*) calloc(1, len);
  if (*buffer == NULL) return TOCK_STATUSCODE_FAIL;

  return TOCK_STATUSCODE_SUCCESS;
}

int libtock_screen_get_bits_per_pixel(libtock_screen_format_t format) {
  switch (format) {
    case MONO:
      return 1;

    case RGB_233:
      return 8;

    case RGB_565:
      return 16;

    case RGB_888:
      return 24;

    case ARGB_8888:
      return 32;

    default:
      return 0;
  }
}



returncode_t libtock_screen_get_supported_resolutions(uint32_t* resolutions) {
  return libtock_screen_command_get_supported_resolutions(resolutions);
}

returncode_t libtock_screen_get_supported_resolution(size_t index, uint32_t* width, uint32_t* height) {
  return libtock_screen_command_get_supported_resolution(index, width, height);
}

returncode_t libtock_screen_get_supported_pixel_formats(uint32_t* formats) {
  return libtock_screen_command_get_supported_pixel_formats(formats);
}

returncode_t libtock_screen_get_supported_pixel_format(size_t index, libtock_screen_format_t* format) {
  returncode_t ret;
  uint32_t f;
  ret     = libtock_screen_command_get_supported_pixel_format(index, &f);
  *format = f;
  return ret;
}

bool libtock_screen_setup_enabled(void) {
  returncode_t err;
  uint32_t enabled;
  err = libtock_screen_command_enabled(&enabled);
  if (err != RETURNCODE_SUCCESS) return false;
  return enabled > 0;
}

returncode_t libtock_screen_set_brightness(uint32_t brightness, libtock_screen_callback_done cb) {
  returncode_t ret;

  ret = libtock_screen_set_upcall(screen_callback_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_command_set_brightness(brightness);
  return ret;
}

returncode_t libtock_screen_invert_on(libtock_screen_callback_done cb) {
  returncode_t ret;

  ret = libtock_screen_set_upcall(screen_callback_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_command_invert_on();
  return ret;
}

returncode_t libtock_screen_invert_off(libtock_screen_callback_done cb) {
  returncode_t ret;

  ret = libtock_screen_set_upcall(screen_callback_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_command_invert_off();
  return ret;
}



returncode_t libtock_screen_get_resolution(uint32_t* width, uint32_t* height) {
  return libtock_screen_command_get_resolution(width, height);
}

returncode_t libtock_screen_set_resolution(uint32_t width, uint32_t height, libtock_screen_callback_done cb) {
  returncode_t ret;

  ret = libtock_screen_set_upcall(screen_callback_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_command_set_resolution(width, height);
  return ret;
}



returncode_t libtock_screen_get_pixel_format(libtock_screen_callback_format cb) {
  returncode_t ret;

  ret = libtock_screen_set_upcall(screen_callback_format, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_command_get_pixel_format();
  return ret;
}

returncode_t libtock_screen_set_pixel_format(libtock_screen_format_t format, libtock_screen_callback_done cb) {
  returncode_t ret;

  ret = libtock_screen_set_upcall(screen_callback_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_command_set_pixel_format((uint32_t) format);
  return ret;
}


returncode_t libtock_screen_get_rotation(libtock_screen_callback_rotation cb) {
  returncode_t ret;

  ret = libtock_screen_set_upcall(screen_callback_rotation, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_command_get_rotation();
  return ret;
}

returncode_t libtock_screen_set_rotation(libtock_screen_rotation_t rotation, libtock_screen_callback_done cb) {
  returncode_t ret;

  ret = libtock_screen_set_upcall(screen_callback_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_command_set_rotation((uint32_t) rotation);
  return ret;
}



returncode_t libtock_screen_set_frame(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                                      libtock_screen_callback_done cb) {
  returncode_t ret;

  ret = libtock_screen_set_upcall(screen_callback_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_command_set_frame(x, y, width, height);
  return ret;
}

static returncode_t screen_set_color(uint8_t* buffer, int buffer_len, int position, size_t color) {
  // TODO color mode
  if (position < buffer_len - 2) {
    buffer[position * 2]     = (color >> 8) & 0xFF;
    buffer[position * 2 + 1] = color & 0xFF;
    return RETURNCODE_SUCCESS;
  } else {
    return RETURNCODE_ESIZE;
  }
}

returncode_t libtock_screen_fill(uint8_t* buffer, int buffer_len, size_t color, libtock_screen_callback_done cb) {
  returncode_t ret;

  ret = screen_set_color(buffer, buffer_len, 0, color);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_readonly_allow(buffer, buffer_len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_set_upcall(screen_callback_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_command_fill();
  return ret;
}

returncode_t libtock_screen_write(uint8_t* buffer, int buffer_len, size_t length, libtock_screen_callback_done cb) {
  returncode_t ret;

  ret = libtock_screen_readonly_allow(buffer, buffer_len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_set_upcall(screen_callback_done, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_command_write(length);
  return ret;
}
