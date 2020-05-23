#include <framebuffer.h>
#include <stdio.h>
#include <stdlib.h>
#include <timer.h>

#define BUFFER_SIZE 10 * 1024

int main(void) {
  printf ("available resolutions\n");
  int resolutions = framebuffer_get_supported_resolutions ();
  for (int idx = 0; idx < resolutions; idx++) {
    size_t width, height;
    framebuffer_get_supported_resolution (idx, &width, &height);
    printf ("  %d x %d\n", width, height);
  }
  printf ("available colors depths\n");
  int pixel_format = framebuffer_get_supported_pixel_formats ();
  for (int idx = 0; idx < pixel_format; idx++) {
    size_t bits = framebuffer_get_bits_per_pixel (framebuffer_get_supported_pixel_format (idx));
    printf ("  %d bpp\n", bits);
  }
  if (framebuffer_init (BUFFER_SIZE) == TOCK_SUCCESS) {
    printf ("framebuffer init\n");
    framebuffer_set_brightness (100);
    framebuffer_set_frame (0, 0, 128, 160);
    framebuffer_fill (0);
    bool invert = false;
    for (int i = 0; ; i++) {
      if (i % 4 == 3) {
        invert = !invert;
        if (invert) {
          framebuffer_invert_on();
        } else {
          framebuffer_invert_off();
        }
      }
      framebuffer_set_rotation (i % 4);
      framebuffer_set_frame (10, 20, 30, 30);
      framebuffer_fill (0xF800);
      framebuffer_set_frame (88, 20, 30, 30);
      framebuffer_fill (0);
      delay_ms (1000);
      framebuffer_set_frame (10, 20, 30, 30);
      framebuffer_fill (0);
      framebuffer_set_frame (88, 20, 30, 30);
      framebuffer_fill (0x07F0);
      delay_ms (1000);
      size_t width, height;
      framebuffer_get_resolution (&width, &height);
      framebuffer_set_frame (0, 0, width, height);
      framebuffer_fill (0x0000);
    }
  }else {
    printf ("buffer allocation failed\n");
  }
  return 0;
}
