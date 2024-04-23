#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <libtock/timer.h>

#include <u8g2-tock.h>
#include <u8g2.h>

u8g2_t u8g2;

#define PI 3.14

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

  int rot = 0;

  while (1) {

    u8g2_ClearBuffer(&u8g2);

    double angle = (((double) rot) / 100.0) * (2 * PI);

    int x = center_x + (radius * cos(angle));
    int y = center_y + (radius * sin(angle));

    u8g2_DrawLine(&u8g2, center_x, center_y, x, y);

    u8g2_SendBuffer(&u8g2);
    delay_ms(200);

    rot = (rot + 1) % 100;
  }
}
