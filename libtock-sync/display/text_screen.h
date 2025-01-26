#pragma once

#include <libtock/display/text_screen.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

returncode_t libtocksync_text_screen_display_on(void);

returncode_t libtocksync_text_screen_display_off(void);

returncode_t libtocksync_text_screen_blink_on(void);

returncode_t libtocksync_text_screen_blink_off(void);

returncode_t libtocksync_text_screen_show_cursor(void);

returncode_t libtocksync_text_screen_hide_cursor(void);

returncode_t libtocksync_text_screen_clear(void);

returncode_t libtocksync_text_screen_home(void);

returncode_t libtocksync_text_screen_set_cursor(uint8_t col, uint8_t row);

returncode_t libtocksync_text_screen_write(uint8_t* buffer, uint32_t buffer_len, uint32_t write_len);

returncode_t libtocksync_text_screen_get_size(uint32_t* width, uint32_t* height);

#ifdef __cplusplus
}
#endif
