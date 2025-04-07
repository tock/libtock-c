#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/display/screen.h>

uint8_t* buffer;

static void write_pixel(int x, int y, uint8_t v) {
  int vertical       = y / 8;
  int byte_index     = x + (vertical * 64);
  int vertical_pixel = y % 8;

  uint8_t original = buffer[byte_index];
  uint8_t new      = (original & ~(1 << vertical_pixel)) | ((v & 0x1) << vertical_pixel);
  buffer[byte_index] = new;
}

static void draw_black_square_middle(uint16_t x, uint16_t y, uint16_t width) {
  if (width < 3) return;

  uint16_t third = width / 3;

  // Write all pixels for the new square.
  for (int i = x + third; i < x + third + third; i++) {
    for (int j = y + third; j < y + third + third; j++) {
      write_pixel(i, j, 0);
    }
  }

  // Recurse for surrounding squares.
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (i == 1 && j == 1) continue;
      draw_black_square_middle(x + (third * i), y + (third * j), third);
    }
  }
}

int main(void) {
  returncode_t err;

  uint32_t width, height;

  err = libtock_screen_get_resolution(&width, &height);
  if (err != RETURNCODE_SUCCESS) return -1;

  // Make width smaller of width and height.
  if (width > height) width = height;

  // Allocate buffer.
  uint32_t buffer_size = width * width;
  buffer = malloc(sizeof(uint8_t) * buffer_size);

  // Fill in the display buffer.
  memset(buffer, 0xff, buffer_size);
  draw_black_square_middle(0, 0, width);
  libtocksync_screen_set_frame(0, 0, width, width);
  libtocksync_screen_write(buffer, buffer_size, buffer_size / 8);

  return 0;
}
