/*L3GD20 3 axis gyroscope and temperature sensor
*
* <https://www.pololu.com/file/0J563/L3GD20.pdf>
*
*/

#pragma once

#include "tock.h"

#define SCREEN_ROTATION_NORMAL 0
#define SCREEN_ROTATION_90 1
#define SCREEN_ROTATION_180 2
#define SCREEN_ROTATION_270 3

#define SCREEN_PIXEL_FORMAT_NONE 0
#define SCREEN_PIXEL_FORMAT_MONO 1
#define SCREEN_PIXEL_FORMAT_RGB_233 2
#define SCREEN_PIXEL_FORMAT_RGB_565 3
#define SCREEN_PIXEL_FORMAT_RGB_888 4
#define SCREEN_PIXEL_FORMAT_ARGB_8888 5

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_FRAMEBUFFER 0x90001

// init
int framebuffer_init (size_t len);
uint8_t * framebuffer_buffer (void);

// query
int framebuffer_get_supported_resolutions (void);
int framebuffer_get_supported_resolution (size_t index, size_t *width, size_t *height);
int framebuffer_get_supported_pixel_formats (void);
int framebuffer_get_supported_pixel_format (size_t index);

// power
int framebuffer_screen_on (void);
int framebuffer_screen_off (void);
int framebuffer_invert_on (void);
int framebuffer_invert_off (void);

// pixel format
int framebuffer_get_bits_per_pixel (size_t format);
int framebuffer_get_pixel_format (void);
int framebuffer_set_pixel_format (size_t format);

// resolution and rotation
int framebuffer_get_resolution (size_t *width, size_t *height);
int framebuffer_set_resolution (size_t width, size_t height);

int framebuffer_get_rotation (void);
int framebuffer_set_rotation (size_t rotation);

// drawing
int framebuffer_set_color (size_t position, size_t color);
int framebuffer_set_window (uint16_t x, uint16_t y, uint16_t width, uint16_t height);
int framebuffer_fill (size_t color);
int framebuffer_write (size_t length);

#ifdef __cplusplus
}
#endif
