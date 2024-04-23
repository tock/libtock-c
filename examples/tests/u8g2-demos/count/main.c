#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock/timer.h>

#include <u8g2-tock.h>
#include <u8g2.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

u8g2_t u8g2;

char buf[20];

int main(void) {

  u8g2_tock_init(&u8g2);

  int width  = u8g2_GetDisplayWidth(&u8g2);
  int height = u8g2_GetDisplayHeight(&u8g2);

  u8g2_SetFont(&u8g2, u8g2_font_timB18_tn);
  u8g2_SetFontPosCenter(&u8g2);

  int count = 0;

  while (1) {
    u8g2_ClearBuffer(&u8g2);

    snprintf(buf, 20, "%i", count);

    int strwidth = u8g2_GetUTF8Width(&u8g2, buf);

    int y_center = height / 2;
    int x        = max((width / 2) - (strwidth / 2), 0);

    u8g2_DrawStr(&u8g2, x, y_center, buf);
    u8g2_SendBuffer(&u8g2);

    count += 1;
    delay_ms(1000);
  }
}
