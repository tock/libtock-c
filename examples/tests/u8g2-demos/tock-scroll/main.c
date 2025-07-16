#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <u8g2-tock.h>
#include <u8g2.h>

#include <libtock-sync/services/alarm.h>

u8g2_t u8g2;

const char* msg = "TOCKOS...";
char buf[20];

int main(void) {

  u8g2_tock_init(&u8g2);

  int height = u8g2_GetDisplayHeight(&u8g2);

  u8g2_SetFont(&u8g2, u8g2_font_profont29_tr);
  u8g2_SetFontPosTop(&u8g2);

  int len    = strlen(msg);
  int cwidth = u8g2_GetUTF8Width(&u8g2, "T");

  while (1) {

    for (int i = 0; i < len; i++) {

      // Rotate characters.
      for (int j = 0; j < len; j++) {
        buf[j] = msg[(j + i) % len];
      }
      buf[len] = '\0';

      // Scroll for one character.
      for (int k = cwidth; k >= 0; k--) {
        u8g2_ClearBuffer(&u8g2);

        u8g2_SetDrawColor(&u8g2, 1);
        u8g2_DrawStr(&u8g2, k, 0, buf);

        // Clear first character to give the hiding effect.
        u8g2_SetDrawColor(&u8g2, 0);
        u8g2_DrawBox(&u8g2, 0, 0, cwidth, height);

        u8g2_SendBuffer(&u8g2);
        libtocksync_alarm_delay_ms(100);
      }

      buf[1] = '\0';
      cwidth = u8g2_GetUTF8Width(&u8g2, buf);
    }
  }
}
