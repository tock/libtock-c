#include <st7735.h>
#include <framebuffer.h>
#include <stdio.h>
#include <stdlib.h>
#include <timer.h>

#define BUFFER_SIZE 10*1024

int main(void) {
  st7735_reset ();
  if (framebuffer_init (BUFFER_SIZE) == TOCK_SUCCESS)
  {
    printf ("framebuffer init\r\n");
    framebuffer_set_window (0, 0, 128, 160);
    framebuffer_fill (0);
    // for (int pos = 0; pos < BUFFER_SIZE/2; pos++) framebuffer_set_color (pos, 0x07E0);
    while (1)
    {
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
    }
    // framebuffer_fill (0);
    // 
    // framebuffer_set_window (0, 0, 128, 160);
    // 
    // framebuffer_write (5000);
  }
  else
  {
    printf ("buffer allocation failed\r\n");
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
