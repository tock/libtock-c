#pragma once

#include "../tock.h"
#include "syscalls/screen_syscalls.h"
#include "screen_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_SCREEN 0x90001

// Callback when an operation has completed.
//
// This callback is used for several operations, but is generic as it only
// returns the returncode for the operation as provided in the upcall.
typedef void (*libtock_screen_callback_done)(returncode_t);

// Callback when retrieving the screen pixel format.
//
// The callback is provided the screen format when called.
typedef void (*libtock_screen_callback_format)(returncode_t, libtock_screen_format_t);

// Callback when retrieving the screen rotation.
//
// The callback includes the rotation angle as an int.
typedef void (*libtock_screen_callback_rotation)(returncode_t, libtock_screen_rotation_t);


// INIT

// Allocate and setup `buffer` with size `len`.
//
// This will allocate len bytes and assign buffer to the array. This buffer can
// be used for drawing the screen.
statuscode_t libtock_screen_buffer_init(size_t len, uint8_t** buffer);

// QUERY

// Check if the screen is setup. Returns `true` if the screen is enabled, or
// `false` otherwise.
bool libtock_screen_setup_enabled(void);

// Get the number of supported resolutions for the screen.
returncode_t libtock_screen_get_supported_resolutions(uint32_t* resolutions);

// Get a particular resolution based on `index`. `width` and `height` will be
// set with the resolution corresponding to that index.
returncode_t libtock_screen_get_supported_resolution(size_t index, uint32_t* width, uint32_t* height);

// Get the number or supported pixel formats.
returncode_t libtock_screen_get_supported_pixel_formats(uint32_t* formats);

// Get the particular format based on `index`. `format` will be set with the
// correct pixel format for that index.
returncode_t libtock_screen_get_supported_pixel_format(size_t index, libtock_screen_format_t* format);

// POWER

// Set the brightness. The callback will be called when the brightness is set.
returncode_t libtock_screen_set_brightness(uint32_t brightness, libtock_screen_callback_done cb);

// Turn on color inversion. The callback will be called when the operation is
// complete.
returncode_t libtock_screen_invert_on(libtock_screen_callback_done cb);

// Turn off color inversion. The callback will be called when the operation is
// complete.
returncode_t libtock_screen_invert_off(libtock_screen_callback_done cb);

// PIXEL FORMAT

// Calculate and return the number of bits per pixel for the given pixel format.
int libtock_screen_get_bits_per_pixel(libtock_screen_format_t format);

// Get the pixel format. The format will be returned in the callback.
returncode_t libtock_screen_get_pixel_format(libtock_screen_callback_format cb);

// Set the pixel format. The callback will be called when the format is set.
returncode_t libtock_screen_set_pixel_format(libtock_screen_format_t format, libtock_screen_callback_done cb);

// RESOLUTION AND ROTATION

// Get the current resolution of the screen.
returncode_t libtock_screen_get_resolution(uint32_t* width, uint32_t* height);

// Set the resolution of the screen to the specified width and height. The
// callback will be called when the resolution is set.
returncode_t libtock_screen_set_resolution(uint32_t width, uint32_t height, libtock_screen_callback_done cb);

// Get the current screen rotation. The rotation will be specified in the
// callback function.
returncode_t libtock_screen_get_rotation(libtock_screen_callback_rotation cb);

// Set the screen rotation. The callback will be called when the operation
// completes.
returncode_t libtock_screen_set_rotation(libtock_screen_rotation_t rotation, libtock_screen_callback_done cb);

// DRAWING

// Set the current drawing frame on the screen.
//
// The frame is specified by the X,Y coordinates as the upper left, and the
// width and height of the frame box. The callback will be called when setting
// the frame is complete.
returncode_t libtock_screen_set_frame(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                                      libtock_screen_callback_done cb);

// Fill the screen to a given color.
//
// The screen data is in buffer. The callback will be called when the screen is
// finished being filled.
returncode_t libtock_screen_fill(uint8_t* buffer, int buffer_len, size_t color, libtock_screen_callback_done cb);

// Write the data in buffer to the screen.
//
// The callback will be called when the screen is finished being updated.
returncode_t libtock_screen_write(uint8_t* buffer, int buffer_len, size_t length, libtock_screen_callback_done cb);

#ifdef __cplusplus
}
#endif
