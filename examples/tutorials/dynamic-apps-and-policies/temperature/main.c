#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/sensors/temperature.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/interface/button.h>

#include <u8g2-tock.h>
#include <u8g2.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

u8g2_t u8g2;

char buf[20];


static void on_button_press(returncode_t err, int button_index, bool pressed) {
  if (err == RETURNCODE_SUCCESS && pressed) {
    printf("[Temp App] Snooped button %i\n", button_index);
  }
}

static void hijack_buttons(void) {
  returncode_t ret;
  int button_count;

  ret = libtock_button_count(&button_count);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Temp App] Error: Unable to access the buttons.\n");
    return;
  }

  for (int i = 0; i < button_count; i++) {
    ret = libtock_button_notify_on_press(i, on_button_press);
    if (ret != RETURNCODE_SUCCESS) return;
  }
}

int main(void) {
  int ret;

  // This app sneakily hijacks all button presses.
  hijack_buttons();

  ret = u8g2_tock_init(&u8g2);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Temp App] Error: Unable to access the screen.\n");
    return -1;
  }

  int width  = u8g2_GetDisplayWidth(&u8g2);
  int height = min(8, u8g2_GetDisplayHeight(&u8g2));

  u8g2_SetFont(&u8g2, u8g2_font_profont10_tf);
  u8g2_SetFontPosCenter(&u8g2);

  while (1) {
    u8g2_ClearBuffer(&u8g2);

    // Set the entire display as on.
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawBox(&u8g2, 0, 0, width - 1, height);
    u8g2_SetDrawColor(&u8g2, 0);

    int temperature_hundredths;
    ret = libtocksync_temperature_read(&temperature_hundredths);
    int temp_whole = temperature_hundredths / 100;
    int temp_tenth = (temperature_hundredths - (temp_whole * 100)) / 10;

    ret = snprintf(buf, 20, "%d.%d%cC", temp_whole, temp_tenth, 0xb0);
    if (ret != 0) buf[19] = '\0';

    int strwidth = u8g2_GetUTF8Width(&u8g2, buf);

    int y_center = (height / 2) + 1;
    int x        = max((width / 2) - (strwidth / 2), 0);

    u8g2_DrawStr(&u8g2, x, y_center, buf);
    u8g2_SendBuffer(&u8g2);

    libtocksync_alarm_delay_ms(3000);
  }
}
