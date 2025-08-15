#include "screen.h"

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


returncode_t libtocksync_screen_set_brightness(uint32_t brightness) {
  returncode_t ret;

  ret = libtock_screen_command_set_brightness(brightness);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_screen_yield_wait_for();
  return ret;
}

returncode_t libtocksync_screen_invert_on(void) {
  returncode_t ret;

  ret = libtock_screen_command_invert_on();
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_screen_yield_wait_for();
  return ret;
}

returncode_t libtocksync_screen_invert_off(void) {
  returncode_t ret;

  ret = libtock_screen_command_invert_off();
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_screen_yield_wait_for();
  return ret;
}

returncode_t libtocksync_screen_get_pixel_format(libtock_screen_format_t* format) {
  returncode_t ret;

  ret = libtock_screen_command_get_pixel_format();
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_screen_yield_wait_for_format(format);
  return ret;
}

returncode_t libtocksync_screen_get_resolution(uint32_t* width, uint32_t* height) {
  return libtock_screen_command_get_resolution(width, height);
}

returncode_t libtocksync_screen_get_rotation(libtock_screen_rotation_t* rotation) {
  returncode_t ret;

  ret = libtock_screen_command_get_rotation();
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_screen_yield_wait_for_rotation(rotation);
  return ret;
}

returncode_t libtocksync_screen_set_rotation(libtock_screen_rotation_t rotation) {
  returncode_t ret;

  ret = libtock_screen_command_set_rotation(rotation);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_screen_yield_wait_for();
  return ret;
}

returncode_t libtocksync_screen_set_frame(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  returncode_t ret;

  ret = libtock_screen_command_set_frame(x, y, width, height);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_screen_yield_wait_for();
  return ret;
}

returncode_t libtocksync_screen_fill(uint8_t* buffer, int buffer_len, size_t color) {
  returncode_t ret;

  ret = screen_set_color(buffer, buffer_len, 0, color);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_set_readonly_allow(buffer, buffer_len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_command_fill();
  if (ret != RETURNCODE_SUCCESS) goto exit;

  ret = libtocksync_screen_yield_wait_for();

exit:
  libtock_screen_set_readonly_allow(NULL, 0);

  return ret;
}

returncode_t libtocksync_screen_write(uint8_t* buffer, int buffer_len, size_t length) {
  returncode_t ret;

  ret = libtock_screen_set_readonly_allow(buffer, buffer_len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_screen_command_write(length);
  if (ret != RETURNCODE_SUCCESS) goto exit;

  ret = libtocksync_screen_yield_wait_for();

exit:
  libtock_screen_set_readonly_allow(NULL, 0);

  return ret;
}
