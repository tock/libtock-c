#include <screen.h>
#include <stdio.h>
#include <stdlib.h>
#include <timer.h>

#include <u8g2-tock.h>
#include <u8g2.h>

u8g2_t u8g2;

int main(void) {
  u8g2_tock_init(&u8g2);
  u8g2_ClearBuffer(&u8g2);

  u8g2_SetFont(&u8g2, u8g2_font_profont29_tr);
  u8g2_SetFontPosTop(&u8g2);
  u8g2_DrawStr(&u8g2, 32, 1, "TOCK");
  u8g2_SetFont(&u8g2, u8g2_font_bubble_tr);
  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_DrawStr(&u8g2, 2, 44, "SECURE");
  u8g2_DrawStr(&u8g2, 42, 64, "OS");

  u8g2_DrawCircle(&u8g2, 17, 12, 7, U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_LOWER_LEFT | U8G2_DRAW_LOWER_RIGHT);
  u8g2_DrawLine(&u8g2, 17, 12, 17, 5);

  u8g2_SendBuffer(&u8g2);
}
