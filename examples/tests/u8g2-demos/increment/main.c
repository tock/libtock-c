#include <stdio.h>
#include <stdlib.h>
#include <u8g2-tock.h>
#include <u8g2.h>

#include <libtock/interface/button.h>

u8g2_t u8g2;

#define BTN_NUM_UP 0
#define BTN_NUM_DOWN 2

const char* UP_ARROW   = "\x93";
const char* DOWN_ARROW = "\x8b";

int counter = 10;

static void button_callback(__attribute__ ((unused)) returncode_t ret, int btn_num, bool pressed) {
  if (pressed) {
    if (btn_num == BTN_NUM_UP) {
      counter += 1;
    } else if (btn_num == BTN_NUM_DOWN) {
      counter -= 1;
    }
  }
}

int main(void) {
  int ret = u8g2_tock_init(&u8g2);
  if (ret != 0) return -100;

  // Setup increment and decrement buttons.
  libtock_button_notify_on_press(BTN_NUM_UP, button_callback);
  libtock_button_notify_on_press(BTN_NUM_DOWN, button_callback);

  // Loop, displaying the updated counter.
  while (1) {
    u8g2_ClearBuffer(&u8g2);

    int width  = u8g2_GetDisplayWidth(&u8g2);
    int height = u8g2_GetDisplayHeight(&u8g2);

    u8g2_SetFont(&u8g2, u8g2_font_cursor_tf);

    // UP ARROW
    u8g2_SetFontPosTop(&u8g2);
    u8g2_DrawStr(&u8g2, width / 2, -7, UP_ARROW);

    // DOWN ARROW
    u8g2_SetFontPosBottom(&u8g2);
    u8g2_DrawStr(&u8g2, width / 2, height + 6, DOWN_ARROW);

    // COUNTER
    u8g2_SetFont(&u8g2, u8g2_font_helvB14_tr);
    u8g2_SetFontPosCenter(&u8g2);

    char buf[100];
    snprintf(buf, 100, "%i", counter);
    int str_width = u8g2_GetStrWidth(&u8g2, buf);
    u8g2_DrawStr(&u8g2, (width - str_width) / 2, height / 2, buf);

    u8g2_SendBuffer(&u8g2);

    // The only pending upcall we have at this point is the button, so this will
    // only return when a button is pressed.
    yield();
  }
}
