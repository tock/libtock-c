#include <libtock/display/syscalls/screen_syscalls.h>

#include "screen.h"

struct screen_done {
  returncode_t ret;
  bool fired;
};
struct screen_format {
  libtock_screen_format_t format;
  returncode_t ret;
  bool fired;
};
struct screen_rotation {
  int rotation;
  returncode_t ret;
  bool fired;
};

static struct screen_done result;
static struct screen_format result_format;
static struct screen_rotation result_rotation;


static void screen_cb_done(returncode_t ret) {
  result.ret   = ret;
  result.fired = true;
}

static void screen_cb_format(returncode_t ret, libtock_screen_format_t format) {
  result_format.ret    = ret;
  result_format.format = format;
  result_format.fired  = true;
}

static void screen_cb_rotation(returncode_t ret, libtock_screen_rotation_t rotation) {
  result_rotation.ret      = ret;
  result_rotation.rotation = rotation;
  result_rotation.fired    = true;
}

bool libtocksync_screen_exists(void) {
  return libtock_screen_driver_exists();
}

returncode_t libtocksync_screen_set_brightness(uint32_t brightness) {
  returncode_t ret;

  result.fired = false;

  ret = libtock_screen_set_brightness(brightness, screen_cb_done);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for the callback.
  yield_for(&result.fired);
  return result.ret;
}

returncode_t libtocksync_screen_invert_on(void) {
  returncode_t ret;

  result.fired = false;

  ret = libtock_screen_invert_on(screen_cb_done);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for the callback.
  yield_for(&result.fired);
  return result.ret;
}

returncode_t libtocksync_screen_invert_off(void) {
  returncode_t ret;

  result.fired = false;

  ret = libtock_screen_invert_on(screen_cb_done);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for the callback.
  yield_for(&result.fired);
  return result.ret;
}

returncode_t libtocksync_screen_get_pixel_format(libtock_screen_format_t* format) {
  returncode_t ret;

  result_format.fired = false;

  ret = libtock_screen_get_pixel_format(screen_cb_format);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for the callback.
  yield_for(&result_format.fired);
  if (result_format.ret != RETURNCODE_SUCCESS) return result_format.ret;

  *format = result_format.format;
  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_screen_get_rotation(libtock_screen_rotation_t* rotation) {
  returncode_t ret;

  result_rotation.fired = false;

  ret = libtock_screen_get_rotation(screen_cb_rotation);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for the callback.
  yield_for(&result_rotation.fired);
  if (result_rotation.ret != RETURNCODE_SUCCESS) return result_rotation.ret;

  *rotation = result_rotation.rotation;
  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_screen_set_rotation(libtock_screen_rotation_t rotation) {
  returncode_t ret;

  result.fired = false;

  ret = libtock_screen_set_rotation(rotation, screen_cb_done);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for the callback.
  yield_for(&result.fired);
  return result.ret;
}

returncode_t libtocksync_screen_set_frame(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  returncode_t ret;

  result.fired = false;

  ret = libtock_screen_set_frame(x, y, width, height, screen_cb_done);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for the callback.
  yield_for(&result.fired);
  return result.ret;
}

returncode_t libtocksync_screen_fill(uint8_t* buffer, int buffer_len, size_t color) {
  returncode_t ret;

  result.fired = false;

  ret = libtock_screen_fill(buffer, buffer_len, color, screen_cb_done);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for the callback.
  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  ret = libtock_screen_set_readonly_allow(NULL, 0);
  return ret;
}

returncode_t libtocksync_screen_write(uint8_t* buffer, int buffer_len, size_t length) {
  returncode_t ret;

  result.fired = false;

  ret = libtock_screen_write(buffer, buffer_len, length, screen_cb_done);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for the callback.
  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  ret = libtock_screen_set_readonly_allow(NULL, 0);
  return ret;
}
