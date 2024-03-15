#include <stdlib.h>

#include "tock.h"
#include "screen_syscalls.h"

bool libtock_screen_exists(void) {
  return driver_exists(DRIVER_NUM_SCREEN);
}

returncode_t libtock_screen_set_upcall(subscribe_upcall cb, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_SCREEN, 0, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_screen_readonly_allow(const void* ptr, size_t size) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_SCREEN, 0, ptr, size);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_screen_command_get_supported_resolutions(uint32_t* resolutions) {
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 11, 0, 0);
  return tock_command_return_u32_to_returncode(cval, resolutions);
}

returncode_t libtock_screen_command_get_supported_resolution(uint32_t index, uint32_t* width, uint32_t* height) {
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 12, index, 0);
  return tock_command_return_u32_u32_to_returncode(cval, width, height)
}

returncode_t libtock_screen_command_get_resolution(uint32_t* width, uint32_t* height) {
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 23, 0, 0);
  return tock_command_return_u32_u32_to_returncode(cval, width, height)
}

returncode_t libtock_screen_command_set_resolution(uint32_t width, uint32_t height) {
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 24, width, height);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_screen_command_get_supported_pixel_formats(uint32_t* formats) {
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 13, 0, 0);
  return tock_command_return_u32_to_returncode(cval, formats);
}

returncode_t libtock_screen_command_get_supported_pixel_format(uint32_t index, uint32_t* format) {
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 14, index, 0);
  return tock_command_return_u32_to_returncode(cval, format);
}

returncode_t libtock_screen_command_enabled(uint32_t* enabled) {
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 1, 0, 0);
  return tock_command_return_u32_to_returncode(cval, enabled);
}

returncode_t libtock_screen_command_set_brightness(uint32_t brightness) {
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 3, brightness, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_screen_command_invert_on(void) {
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 4, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_screen_command_invert_off(void) {
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 5, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_screen_command_get_pixel_format(void) {
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 25, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_screen_command_set_pixel_format(uint32_t format) {
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 26, format, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_screen_command_get_rotation(void) {
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 21, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_screen_command_set_rotation(uint32_t rotation) {
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 22, rotation, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_screen_command_set_frame(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  uint32_t arg1 = ((x & 0xFFFF) << 16) | (y & 0xFFFF);
  uint32_t arg2 = ((width & 0xFFFF) << 16) | (height & 0xFFFF);
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 100, arg1, arg2);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_screen_command_write(uint32_t length) {
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 200, length, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_screen_command_fill(void) {
  syscall_return_t cval = command(DRIVER_NUM_SCREEN, 300, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
