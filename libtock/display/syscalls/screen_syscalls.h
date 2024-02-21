#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_SCREEN 0x90001

returncode_t libtock_screen_set_upcall(subscribe_upcall cb, void* opaque);

returncode_t libtock_screen_readonly_allow(const void* ptr, size_t size);

returncode_t libtock_screen_command_get_supported_resolutions(uint32_t* resolutions);

returncode_t libtock_screen_command_get_supported_resolution(size_t index, uint32_t* width, uint32_t* height);

returncode_t libtock_screen_command_get_resolution(uint32_t* width, uint32_t* height);

returncode_t libtock_screen_command_set_resolution(uint32_t width, uint32_t height);

returncode_t libtock_screen_command_get_supported_pixel_formats(uint32_t* formats);

returncode_t libtock_screen_command_get_supported_pixel_format(uint32_t* format);

returncode_t libtock_screen_command_enabled(uint32_t* enabled);

returncode_t libtock_screen_command_set_brightness(void);

returncode_t libtock_screen_command_invert_on(void);

returncode_t libtock_screen_command_invert_off(void);

returncode_t libtock_screen_command_get_pixel_format(void);

returncode_t libtock_screen_command_set_pixel_format(uint32_t format);

returncode_t libtock_screen_command_set_rotation(void);

returncode_t libtock_screen_command_set_rotation(uint32_t rotation);

returncode_t libtock_screen_command_set_frame(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

returncode_t libtock_screen_command_write(uint32_t length);

returncode_t libtock_screen_command_fill(void);

#ifdef __cplusplus
}
#endif

