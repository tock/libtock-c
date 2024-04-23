#include <screen.h>
#include <stdio.h>
#include <stdlib.h>
#include <timer.h>

#define BUFFER_SIZE 10 * 1024

int main(void) {
  int err;

  err = screen_init(BUFFER_SIZE);
  if (err < 0) {
    printf("buffer allocation failed\n");
    return -1;
  }

  printf("screen init\n");

  size_t width, height;
  screen_get_resolution(&width, &height);

  screen_set_brightness(32000);

  screen_set_frame(0, 0, width, height);
  screen_fill(0);

  size_t char_width = width / 4;
  size_t x, y, w, h;
  size_t x_offset = 0;

  // T

  x = x_offset + 1;
  y = 0;
  w = char_width - 2;
  h = height / 5;
  screen_set_frame(x, y, w, h);
  screen_fill(0xFFFF);

  x = x_offset + char_width / 3;
  y = 0;
  w = char_width / 3;
  h = height;
  screen_set_frame(x, y, w, h);
  screen_fill(0xFFFF);

  // O
  x_offset = char_width;

  x = x_offset + 1;
  y = 0;
  w = char_width - 2;
  h = height;
  screen_set_frame(x, y, w, h);
  screen_fill(0xFFFF);

  x = x_offset + (char_width / 3);
  y = height / 5;
  w = char_width / 3;
  h = (height / 5) * 4;
  screen_set_frame(x, y, w, h);
  screen_fill(0);

  // C
  x_offset = char_width * 2;

  x = x_offset + 1;
  y = 0;
  w = char_width - 2;
  h = height;
  screen_set_frame(x, y, w, h);
  screen_fill(0xFFFF);

  x = x_offset + (char_width / 3);
  y = height / 5;
  w = 2 * (char_width / 3) + 1;
  h = (height / 5) * 4;
  screen_set_frame(x, y, w, h);
  screen_fill(0);

  // K
  x_offset = char_width * 3;

  x = x_offset + 1;
  y = 0;
  w = char_width / 3;
  h = height;
  screen_set_frame(x, y, w, h);
  screen_fill(0xFFFF);

  x = x_offset + 1;
  y = (height / 5) * 2;
  w = char_width - 2;
  h = height / 5;
  screen_set_frame(x, y, w, h);
  screen_fill(0xFFFF);

  x = x_offset + ((char_width / 3) * 2);
  y = 16;
  w = char_width / 3;
  h = 16;
  screen_set_frame(x, y, w, h);
  screen_fill(0xFFFF);

  x = x_offset + ((char_width / 3) * 2);
  y = 32;
  w = char_width / 3;
  h = 16;
  screen_set_frame(x, y, w, h);
  screen_fill(0xFFFF);

  printf("TOCK on screen!\n");

  return 0;
}
