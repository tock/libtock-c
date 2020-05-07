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

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_FRAMEBUFFER 0x90001

int framebuffer_count_resolutions (void);
int framebuffer_get_resolution_size (size_t index, size_t *width, size_t *height);

int framebuffer_count_color_depths (void);
int framebuffer_get_color_depth_bits (size_t index, size_t *depth);

int framebuffer_init (size_t len);

int framebuffer_get_resolution (size_t *width, size_t *height);
int framebuffer_set_resolution (size_t width, size_t height);

int framebuffer_get_color_depth (size_t *bits);
int framebuffer_set_color_depth (size_t bits);

int framebuffer_get_rotation (size_t *rotation);
int framebuffer_set_rotation (size_t rotation);

int framebuffer_set_color (size_t position, size_t color);

int framebuffer_set_window (uint16_t x, uint16_t y, uint16_t width, uint16_t height);
int framebuffer_fill (size_t color);
int framebuffer_write (size_t length);

#ifdef __cplusplus
}
#endif
