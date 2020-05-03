/*L3GD20 3 axis gyroscope and temperature sensor
*
* <https://www.pololu.com/file/0J563/L3GD20.pdf>
*
*/

#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_FRAMEBUFFER 0x90001

int framebuffer_init (size_t len);
int framebuffer_set_color (size_t position, size_t color);
int framebuffer_set_window (uint16_t x, uint16_t y, uint16_t width, uint16_t height);
int framebuffer_fill (size_t color);
int framebuffer_write (size_t length);

#ifdef __cplusplus
}
#endif
