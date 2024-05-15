#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_TEXT_SCREEN 0x90003

returncode_t libtock_text_screen_set_upcall(subscribe_upcall callback, void* opaque);

returncode_t libtock_text_screen_set_readonly_allow(const uint8_t* ptr, uint32_t size);

returncode_t libtock_text_screen_command_get_size(void);

returncode_t libtock_text_screen_command_on(void);

returncode_t libtock_text_screen_command_off(void);

returncode_t libtock_text_screen_command_blink_on(void);

returncode_t libtock_text_screen_command_blink_off(void);

returncode_t libtock_text_screen_command_show_cursor(void);

returncode_t libtock_text_screen_command_hide_cursor(void);

returncode_t libtock_text_screen_command_write(uint32_t len);

returncode_t libtock_text_screen_command_clear(void);

returncode_t libtock_text_screen_command_home(void);

returncode_t libtock_text_screen_command_set_cursor(uint32_t col, uint32_t row);

#ifdef __cplusplus
}
#endif
