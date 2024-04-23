#include <stdio.h>
#include <stdlib.h>

#include <libtock/timer.h>

#include <u8g2-tock.h>
#include <u8g2.h>

u8g2_t u8g2;

int main(void) {
  u8g2_tock_init(&u8g2);

  int width  = u8g2_GetDisplayWidth(&u8g2);
  int height = u8g2_GetDisplayHeight(&u8g2);

  int center_x = width / 2;
  int center_y = height / 2;
  int radius   = 0;

  if (width < height) {
    radius = center_x - 1;
  } else {
    radius = center_y - 1;
  }

  while (1) {
    for (int r = 0; r < radius; r++) {
      u8g2_ClearBuffer(&u8g2);
      u8g2_DrawCircle(&u8g2, center_x, center_y, r, U8G2_DRAW_ALL);
      u8g2_SendBuffer(&u8g2);

      delay_ms(100);
    }
    for (int r = radius - 1; r > 0; r--) {
      u8g2_ClearBuffer(&u8g2);
      u8g2_DrawCircle(&u8g2, center_x, center_y, r, U8G2_DRAW_ALL);
      u8g2_SendBuffer(&u8g2);

      delay_ms(100);
    }
  }
}
