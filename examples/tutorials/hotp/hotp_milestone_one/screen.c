#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <u8g2-tock.h>
#include <u8g2.h>

#include "screen.h"
#include "step0.h"

u8g2_t u8g2;
bool inited = false;

int display_hotp_keys(hotp_key_t* hotp_key, int num_keys) {
  // Only init if not previously init-ed.
  if (!inited) {
    int ret = u8g2_tock_init(&u8g2);
    if (ret != 0) {
      return -1;
    }
    inited = true;
  }

  // int height = u8g2_GetDisplayHeight(&u8g2);
  int width = u8g2_GetDisplayWidth(&u8g2);

  u8g2_SetFont(&u8g2, u8g2_font_profont17_tr);
  u8g2_SetFontPosTop(&u8g2);

  u8g2_ClearBuffer(&u8g2);

  // Draw title.
  u8g2_DrawStr(&u8g2, 0, 0, "HOTP App");

  // Draw line below title.
  int title_height = u8g2_GetMaxCharHeight(&u8g2);
  u8g2_DrawHLine(&u8g2, 0, title_height, width);

  // Draw for each key.
  u8g2_SetFont(&u8g2, u8g2_font_helvR08_tr);
  int lines_start = title_height + 1;
  int offset      = u8g2_GetMaxCharHeight(&u8g2) + 1;

  for (int i = 0; i < num_keys; i++) {
    char buf[100];

    int y_pos = lines_start + (offset * i);

    if (hotp_key[i].len == 0) {
      snprintf(buf, 100, "Key %i: (unused)", i);
    } else {
      snprintf(buf, 100, "Key %i: %li", i, (uint32_t) hotp_key[i].counter);
    }
    u8g2_DrawStr(&u8g2, 0, y_pos, buf);
  }

  // Write to display.
  u8g2_SendBuffer(&u8g2);

  return 0;
}
