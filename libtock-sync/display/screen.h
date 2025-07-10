#pragma once

#include <libtock/display/screen_types.h>
#include <libtock/tock.h>

#include "syscalls/screen_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_screen_exists(void);

// Allocate and setup `buffer` with size `len`.
//
// This will allocate len bytes and assign buffer to the array. This buffer can
// be used for drawing the screen.
statuscode_t libtocksync_screen_buffer_init(size_t len, uint8_t** buffer);

// Get the number of supported resolutions for the screen.
returncode_t libtocksync_screen_get_supported_resolutions(uint32_t* resolutions);

// Get a particular resolution based on `index`. `width` and `height` will be
// set with the resolution corresponding to that index.
returncode_t libtocksync_screen_get_supported_resolution(size_t index, uint32_t* width, uint32_t* height);

// Get the number or supported pixel formats.
returncode_t libtocksync_screen_get_supported_pixel_formats(uint32_t* formats);

// Get the particular format based on `index`. `format` will be set with the
// correct pixel format for that index.
returncode_t libtocksync_screen_get_supported_pixel_format(size_t index, libtock_screen_format_t* format);

// Calculate and return the number of bits per pixel for the given pixel format.
int libtocksync_screen_get_bits_per_pixel(libtock_screen_format_t format);

// Set the screen brightness.
returncode_t libtocksync_screen_set_brightness(uint32_t brightness);

// Invert the screen.
returncode_t libtocksync_screen_invert_on(void);

// Disable the screen inversion.
returncode_t libtocksync_screen_invert_off(void);

// Get the current pixel format used by the screen.
returncode_t libtocksync_screen_get_pixel_format(libtock_screen_format_t* format);

// Get the current screen resolution.
returncode_t libtocksync_screen_get_resolution(uint32_t* width, uint32_t* height);

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
