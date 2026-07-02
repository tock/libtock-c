#include <libtock/defer.h>
#include <libtock/display/syscalls/text_screen_syscalls.h>

#include "syscalls/text_screen_syscalls.h"
#include "text_screen.h"

bool libtocksync_text_screen_exists(void) {
  return libtock_text_screen_driver_exists();
}

returncode_t libtocksync_text_screen_display_on(void) {
  returncode_t ret;
  ret = libtock_text_screen_command_on();
  if (ret != RETURNCODE_SUCCESS) return ret;
  return libtocksync_text_screen_yield_wait_for_done();
}

returncode_t libtocksync_text_screen_display_off(void) {
  returncode_t ret;
  ret = libtock_text_screen_command_off();
  if (ret != RETURNCODE_SUCCESS) return ret;
  return libtocksync_text_screen_yield_wait_for_done();
}

returncode_t libtocksync_text_screen_blink_on(void) {
  returncode_t ret;
  ret = libtock_text_screen_command_blink_on();
  if (ret != RETURNCODE_SUCCESS) return ret;
  return libtocksync_text_screen_yield_wait_for_done();
}

returncode_t libtocksync_text_screen_blink_off(void) {
  returncode_t ret;
  ret = libtock_text_screen_command_blink_off();
  if (ret != RETURNCODE_SUCCESS) return ret;
  return libtocksync_text_screen_yield_wait_for_done();
}

returncode_t libtocksync_text_screen_show_cursor(void) {
  returncode_t ret;
  ret = libtock_text_screen_command_show_cursor();
  if (ret != RETURNCODE_SUCCESS) return ret;
  return libtocksync_text_screen_yield_wait_for_done();
}

returncode_t libtocksync_text_screen_hide_cursor(void) {
  returncode_t ret;
  ret = libtock_text_screen_command_hide_cursor();
  if (ret != RETURNCODE_SUCCESS) return ret;
  return libtocksync_text_screen_yield_wait_for_done();
}

returncode_t libtocksync_text_screen_clear(void) {
  returncode_t ret;
  ret = libtock_text_screen_command_clear();
  if (ret != RETURNCODE_SUCCESS) return ret;
  return libtocksync_text_screen_yield_wait_for_done();
}

returncode_t libtocksync_text_screen_home(void) {
  returncode_t ret;
  ret = libtock_text_screen_command_home();
  if (ret != RETURNCODE_SUCCESS) return ret;
  return libtocksync_text_screen_yield_wait_for_done();
}

returncode_t libtocksync_text_screen_set_cursor(uint8_t col, uint8_t row) {
  returncode_t ret;
  ret = libtock_text_screen_command_set_cursor((uint32_t) col, (uint32_t) row);
  if (ret != RETURNCODE_SUCCESS) return ret;
  return libtocksync_text_screen_yield_wait_for_done();
}

returncode_t libtocksync_text_screen_write(uint8_t* buffer, uint32_t buffer_len, uint32_t write_len) {
  returncode_t ret;

  ret = libtock_text_screen_set_readonly_allow(buffer, buffer_len);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_text_screen_set_readonly_allow(NULL, 0);
  }

  ret = libtock_text_screen_command_write(write_len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  return libtocksync_text_screen_yield_wait_for_done();
}

returncode_t libtocksync_text_screen_get_size(uint32_t* width, uint32_t* height) {
  returncode_t ret;
  ret = libtock_text_screen_command_get_size();
  if (ret != RETURNCODE_SUCCESS) return ret;
  return libtocksync_text_screen_yield_wait_for_size(width, height);
}
