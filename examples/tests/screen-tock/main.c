#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/display/screen.h>
#include <libtock/timer.h>

#define BUFFER_SIZE 10 * 1024

uint8_t screen_buffer[BUFFER_SIZE];

int main(void) {
  printf("[SCREEN-TOCK] Test Screen Display\n");

  uint32_t width, height;
  libtock_screen_get_resolution(&width, &height);

  libtocksync_screen_set_brightness(32000);

  libtocksync_screen_set_frame(0, 0, width, height);
  libtocksync_screen_fill(screen_buffer, BUFFER_SIZE, 0);

  size_t char_width = width / 4;
  size_t x, y, w, h;
  size_t x_offset = 0;

  // T

  x = x_offset + 1;
  y = 0;
  w = char_width - 2;
  h = height / 5;
  libtocksync_screen_set_frame(x, y, w, h);
  libtocksync_screen_fill(screen_buffer, BUFFER_SIZE, 0xFFFF);

  x = x_offset + char_width / 3;
  y = 0;
  w = char_width / 3;
  h = height;
  libtocksync_screen_set_frame(x, y, w, h);
  libtocksync_screen_fill(screen_buffer, BUFFER_SIZE, 0xFFFF);

  // O
  x_offset = char_width;

  x = x_offset + 1;
  y = 0;
  w = char_width - 2;
  h = height;
  libtocksync_screen_set_frame(x, y, w, h);
  libtocksync_screen_fill(screen_buffer, BUFFER_SIZE, 0xFFFF);

  x = x_offset + (char_width / 3);
  y = height / 5;
  w = char_width / 3;
  h = (height / 5) * 4;
  libtocksync_screen_set_frame(x, y, w, h);
  libtocksync_screen_fill(screen_buffer, BUFFER_SIZE, 0);

  // C
  x_offset = char_width * 2;

  x = x_offset + 1;
  y = 0;
  w = char_width - 2;
  h = height;
  libtocksync_screen_set_frame(x, y, w, h);
  libtocksync_screen_fill(screen_buffer, BUFFER_SIZE, 0xFFFF);

  x = x_offset + (char_width / 3);
  y = height / 5;
  w = 2 * (char_width / 3) + 1;
  h = (height / 5) * 4;
  libtocksync_screen_set_frame(x, y, w, h);
  libtocksync_screen_fill(screen_buffer, BUFFER_SIZE, 0);

  // K
  x_offset = char_width * 3;

  x = x_offset + 1;
  y = 0;
  w = char_width / 3;
  h = height;
  libtocksync_screen_set_frame(x, y, w, h);
  libtocksync_screen_fill(screen_buffer, BUFFER_SIZE, 0xFFFF);

  x = x_offset + 1;
  y = (height / 5) * 2;
  w = char_width - 2;
  h = height / 5;
  libtocksync_screen_set_frame(x, y, w, h);
  libtocksync_screen_fill(screen_buffer, BUFFER_SIZE, 0xFFFF);

  x = x_offset + ((char_width / 3) * 2);
  y = 16;
  w = char_width / 3;
  h = 16;
  libtocksync_screen_set_frame(x, y, w, h);
  libtocksync_screen_fill(screen_buffer, BUFFER_SIZE, 0xFFFF);

  x = x_offset + ((char_width / 3) * 2);
  y = 32;
  w = char_width / 3;
  h = 16;
  libtocksync_screen_set_frame(x, y, w, h);
  libtocksync_screen_fill(screen_buffer, BUFFER_SIZE, 0xFFFF);

  printf("TOCK on screen!\n");

  return 0;
}
