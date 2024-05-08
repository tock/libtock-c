#pragma once

#include <libtock/display/screen.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Set the screen brightness.
returncode_t libtocksync_screen_set_brightness(uint32_t brightness);

// Invert the screen.
returncode_t libtocksync_screen_invert_on(void);

// Disable the screen inversion.
returncode_t libtocksync_screen_invert_off(void);

// Get the current pixel format used by the screen.
returncode_t libtocksync_screen_get_pixel_format(libtock_screen_format_t* format);

// Get the current screen rotation.
returncode_t libtocksync_screen_get_rotation(libtock_screen_rotation_t* rotation);

// Rotate the screen.
returncode_t libtocksync_screen_set_rotation(libtock_screen_rotation_t rotation);

// Set the active screen frame.
returncode_t libtocksync_screen_set_frame(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

// Fill the screen.
returncode_t libtocksync_screen_fill(uint8_t* buffer, int buffer_len, size_t color);

// Write the buffer to the screen.
returncode_t libtocksync_screen_write(uint8_t* buffer, int buffer_len, size_t length);

#ifdef __cplusplus
}
#endif