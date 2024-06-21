
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include <u8g2.h>



static int write_count = 0;


// MOVED FROM u8x8_display.c
void u8x8_RefreshDisplay(u8x8_t *u8x8)
{
  // u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_REFRESH, 0, NULL);

  char name[100];
  snprintf(name, 100, "out/display_%05i.tga", write_count);

  u8x8_SaveBitmapTGA(u8x8, name);

  write_count += 1;
}


int u8g2_tock_init(u8g2_t *u8g2) {
  u8g2_SetupBitmap(u8g2, &u8g2_cb_r0, 128, 64);
  u8x8_InitDisplay(u8g2_GetU8x8(u8g2));
  return 0;
}


int libtocksync_alarm_delay_ms(uint32_t ms) {
  printf("delay %ims\n", ms);
  usleep(ms * 1000);
  return 0;
}

void yield(void) {
  usleep(100000000);
}

int libtock_button_notify_on_press(int button_num, void* cb){}
