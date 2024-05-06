#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/display/screen.h>
#include <libtock-sync/services/alarm.h>

#define BUFFER_SIZE 10 * 1024
uint8_t* buffer;

int main(void) {
  int err;

  printf("available resolutions\n");
  uint32_t resolutions;
  err = libtock_screen_get_supported_resolutions(&resolutions);
  if (err < 0) return -1;
  for (uint32_t idx = 0; idx < resolutions; idx++) {
    uint32_t width, height;
    libtock_screen_get_supported_resolution(idx, &width, &height);
    printf("  %ld x %ld\n", width, height);
  }

  printf("available colors depths\n");
  uint32_t pixel_format;
  err = libtock_screen_get_supported_pixel_formats(&pixel_format);
  if (err < 0) return -1;
  for (uint32_t idx = 0; idx < pixel_format; idx++) {
    libtock_screen_format_t format;
    libtock_screen_get_supported_pixel_format(idx, &format);
    int bits = libtock_screen_get_bits_per_pixel(format);
    printf("  %d bpp\n", bits);
  }

  err = libtock_screen_buffer_init(BUFFER_SIZE, &buffer);
  if (err < 0) {
    printf("buffer allocation failed\n");
    return -1;
  }

  printf("screen init\n");
  libtocksync_screen_set_brightness(100);
  uint32_t width, height;
  libtock_screen_get_resolution(&width, &height);
  libtocksync_screen_set_frame(0, 0, width, height);
  libtocksync_screen_fill(buffer, BUFFER_SIZE, 0);
  bool invert = false;
  for (int i = 0; ; i++) {
    if (i % 4 == 3) {
      invert = !invert;
      if (invert) {
        libtocksync_screen_invert_on();
      } else {
        libtocksync_screen_invert_off();
      }
    }
    libtocksync_screen_set_rotation(i % 4);
    libtocksync_screen_set_frame(10, 20, 30, 30);
    libtocksync_screen_fill(buffer, BUFFER_SIZE, 0xF800);
    libtocksync_screen_set_frame(88, 20, 30, 30);
    libtocksync_screen_fill(buffer, BUFFER_SIZE, 0);
    libtocksync_alarm_delay_ms(1000);
    libtocksync_screen_set_frame(10, 20, 30, 30);
    libtocksync_screen_fill(buffer, BUFFER_SIZE, 0);
    libtocksync_screen_set_frame(88, 20, 30, 30);
    libtocksync_screen_fill(buffer, BUFFER_SIZE, 0x07F0);
    libtocksync_alarm_delay_ms(1000);
    libtocksync_screen_set_frame(0, 0, width, height);
    libtocksync_screen_fill(buffer, BUFFER_SIZE, 0x0000);
  }

  return 0;
}
