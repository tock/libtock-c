#include "text_screen.h"

struct text_screen_data {
  bool fired;
  returncode_t ret;
};

struct text_screen_size_data {
  bool fired;
  returncode_t ret;
  uint32_t width;
  uint32_t height;
};

static struct text_screen_data result = {.fired = false};
static struct text_screen_size_data result_size = {.fired = false};

static void text_screen_cb(returncode_t ret) {
  result.fired = true;
  result.ret   = ret;
}

static void text_screen_size_cb(returncode_t ret, uint32_t width, uint32_t height) {
  result_size.fired  = true;
  result_size.ret    = ret;
  result_size.width  = width;
  result_size.height = height;
}

static returncode_t text_screen_op(returncode_t (*op)(void (*)(returncode_t))) {
  returncode_t ret;
  result.fired = false;

  ret = op(text_screen_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&result.fired);
  return result.ret;
}

returncode_t libtocksync_text_screen_display_on(void) {
  return text_screen_op(libtock_text_screen_display_on);
}

returncode_t libtocksync_text_screen_display_off(void) {
  return text_screen_op(libtock_text_screen_display_off);
}

returncode_t libtocksync_text_screen_blink_on(void) {
  return text_screen_op(libtock_text_screen_blink_on);
}

returncode_t libtocksync_text_screen_blink_off(void) {
  return text_screen_op(libtock_text_screen_blink_off);
}

returncode_t libtocksync_text_screen_show_cursor(void) {
  return text_screen_op(libtock_text_screen_show_cursor);
}

returncode_t libtocksync_text_screen_hide_cursor(void) {
  return text_screen_op(libtock_text_screen_hide_cursor);
}

returncode_t libtocksync_text_screen_clear(void) {
  return text_screen_op(libtock_text_screen_clear);
}

returncode_t libtocksync_text_screen_home(void) {
  return text_screen_op(libtock_text_screen_home);
}

returncode_t libtocksync_text_screen_set_cursor(uint8_t col, uint8_t row) {
  returncode_t ret;
  result.fired = false;

  ret = libtock_text_screen_set_cursor(col, row, text_screen_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&result.fired);
  return result.ret;
}

returncode_t libtocksync_text_screen_write(uint8_t* buffer, uint32_t buffer_len, uint32_t write_len) {
  returncode_t ret;
  result.fired = false;

  ret = libtock_text_screen_write(buffer, buffer_len, write_len, text_screen_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&result.fired);

  ret = libtock_text_screen_set_readonly_allow(NULL, 0);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return result.ret;
}

returncode_t libtocksync_text_screen_get_size(uint32_t* width, uint32_t* height) {
  returncode_t ret;
  result.fired = false;

  ret = libtock_text_screen_get_size(text_screen_size_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&result_size.fired);
  if (result_size.ret != RETURNCODE_SUCCESS) return result_size.ret;

  *width  = result_size.width;
  *height = result_size.height;

  return result_size.ret;
}
