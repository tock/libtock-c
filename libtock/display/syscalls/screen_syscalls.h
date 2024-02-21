#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_SCREEN 0x90001

// Set the upcall for screen upcalls.
returncode_t libtock_screen_set_upcall(subscribe_upcall cb, void* opaque);

// Allow a buffer used to write the screen.
returncode_t libtock_screen_readonly_allow(const void* ptr, size_t size);

// Get the number of supported resolutions.
returncode_t libtock_screen_command_get_supported_resolutions(uint32_t* resolutions);

// Get the supported resolution at the given index.
returncode_t libtock_screen_command_get_supported_resolution(uint32_t index, uint32_t* width, uint32_t* height);

// Get the current screen resolution.
returncode_t libtock_screen_command_get_resolution(uint32_t* width, uint32_t* height);

// Set the screen resolution.
returncode_t libtock_screen_command_set_resolution(uint32_t width, uint32_t height);

// Get the number of supported pixel formats.
returncode_t libtock_screen_command_get_supported_pixel_formats(uint32_t* formats);

// Get the specific supported pixel format.
returncode_t libtock_screen_command_get_supported_pixel_format(uint32_t index, uint32_t* format);

// Check if screen is enabled.
returncode_t libtock_screen_command_enabled(uint32_t* enabled);

// Set the brightness.
returncode_t libtock_screen_command_set_brightness(uint32_t brightness);

// Invert the screen.
returncode_t libtock_screen_command_invert_on(void);

// Disable the screen inversion.
returncode_t libtock_screen_command_invert_off(void);

// Get the current pixel format.
returncode_t libtock_screen_command_get_pixel_format(void);

// Set the pixel format.
returncode_t libtock_screen_command_set_pixel_format(uint32_t format);

// Get the current screen rotation.
returncode_t libtock_screen_command_get_rotation(void);

// Set the screen rotation.
returncode_t libtock_screen_command_set_rotation(uint32_t rotation);

// Set the current active screen frame.
returncode_t libtock_screen_command_set_frame(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

// Write the allowed buffer to the screen.
returncode_t libtock_screen_command_write(uint32_t length);

// Fill the screen from the allowed buffer.
returncode_t libtock_screen_command_fill(void);

#ifdef __cplusplus
}
#endif

