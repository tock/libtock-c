#include <framebuffer.h>
#include <stdio.h>
#include <stdlib.h>
#include <timer.h>

#define BUFFER_SIZE 10 * 1024

int main(void) {
  framebuffer_screen_init ();
  framebuffer_screen_on ();
  printf ("available resolutions\n");
  int resolutions = framebuffer_count_resolutions ();
  for (int idx = 0; idx < resolutions; idx++) {
    size_t width, height;
    framebuffer_get_resolution_size (idx, &width, &height);
    printf ("  %d x %d\n", width, height);
  }
  printf ("available colors depths\n");
  int color_depths = framebuffer_count_color_depths ();
  for (int idx = 0; idx < color_depths; idx++) {
    size_t bits;
    framebuffer_get_color_depth_bits (idx, &bits);
    printf ("  %d bpp\n", bits);
  }
  if (framebuffer_init (BUFFER_SIZE) == TOCK_SUCCESS) {
    printf ("framebuffer init\n");
    framebuffer_set_window (0, 0, 128, 160);
    framebuffer_fill (0);
    bool invert = false;
    // for (int pos = 0; pos < BUFFER_SIZE/2; pos++) framebuffer_set_color (pos, 0x07E0);
    for (int i = 0; ; i++) {
      if (i % 4 == 3) {
        invert = !invert;
        if (invert) {
          framebuffer_invert_on();
        }else  {
          framebuffer_invert_off();
        }
      }
      framebuffer_set_rotation (i % 4);
      framebuffer_set_window (10, 20, 30, 30);
      framebuffer_fill (0xF800);
      framebuffer_set_window (88, 20, 30, 30);
      framebuffer_fill (0);
      delay_ms (1000);
      framebuffer_set_window (10, 20, 30, 30);
      framebuffer_fill (0);
      framebuffer_set_window (88, 20, 30, 30);
      framebuffer_fill (0x07F0);
      delay_ms (1000);
      size_t width, height;
      framebuffer_get_resolution (&width, &height);
      framebuffer_set_window (0, 0, width, height);
      framebuffer_fill (0x0000);
    }
    // framebuffer_fill (0);
    //
    // framebuffer_set_window (0, 0, 128, 160);
    //
    // framebuffer_write (5000);
  }else  {
    printf ("buffer allocation failed\n");
  }
  // while (1)
  // {
  //   st7735_fill (ST7735_COLOR_BLACK);
  //   delay_ms (1000);
  //   st7735_fill (ST7735_COLOR_BLUE);
  //   delay_ms (1000);
  // }

  return 0;
}
