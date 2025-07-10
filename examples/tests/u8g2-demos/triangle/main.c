#include <stdio.h>
#include <stdlib.h>
#include <u8g2-tock.h>
#include <u8g2.h>

#include <libtock-sync/services/alarm.h>

u8g2_t u8g2;

int main(void) {
  u8g2_tock_init(&u8g2);
  u8g2_ClearBuffer(&u8g2);

  int width  = u8g2_GetDisplayWidth(&u8g2);
  int height = u8g2_GetDisplayHeight(&u8g2);

  int center_x = width / 2;

  u8g2_DrawTriangle(&u8g2, center_x, 0, 0, height - 1, width - 1, height - 1);

  u8g2_SendBuffer(&u8g2);
}
