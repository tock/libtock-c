#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>

#include <u8g2-tock.h>
#include <u8g2.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

u8g2_t u8g2;

char buf[20];

int main(void) {
  int ret;

  ret = u8g2_tock_init(&u8g2);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[COUNTER] Error: Unable to access the screen.\n");
    return -1;
  }

  int width  = u8g2_GetDisplayWidth(&u8g2);
  int height = min(8, u8g2_GetDisplayHeight(&u8g2));

  u8g2_SetFont(&u8g2, u8g2_font_chroma48medium8_8r);
  u8g2_SetFontPosCenter(&u8g2);


  int count = 0;

  while (1) {
    u8g2_ClearBuffer(&u8g2);

    // Set the entire display as on.
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawBox(&u8g2, 0, 0, width-1, height);
    u8g2_SetDrawColor(&u8g2, 0);

    snprintf(buf, 20, "Count: %i", count);

    int strwidth = u8g2_GetUTF8Width(&u8g2, buf);

    int y_center = (height / 2) + 1;
    int x        = max((width / 2) - (strwidth / 2), 0);

    u8g2_DrawStr(&u8g2, x, y_center, buf);
    u8g2_SendBuffer(&u8g2);

    count += 1;
    libtocksync_alarm_delay_ms(3000);
  }
}
