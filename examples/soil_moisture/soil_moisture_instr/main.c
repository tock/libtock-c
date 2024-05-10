#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <u8g2-tock.h>
#include <u8g2.h>

#include "sensor_service.h"

// Water if the soil moisture is below 45.5%.
#define WATER_THRESHOLD_TENTH_PERCENT 455

#define max(a, b) ((a) > (b) ? (a) : (b))

char ipc_buf[64] __attribute__((aligned(64)));

u8g2_t u8g2;

int display_width;
int display_height;


// Notify viewers to water the plant!
static void water_me(void) {
  u8g2_ClearBuffer(&u8g2);

  char buf[20];
  snprintf(buf, 20, "Water Me!");

  int strwidth = u8g2_GetUTF8Width(&u8g2, buf);

  int y_center = display_height / 2;
  int x        = max((display_width / 2) - (strwidth / 2), 0);

  u8g2_DrawStr(&u8g2, x, y_center, buf);
  u8g2_SendBuffer(&u8g2);
}

// No water needed, just clear the screen.
static void all_good(void) {
  u8g2_ClearBuffer(&u8g2);
  u8g2_SendBuffer(&u8g2);
}

static void ipc_callback(uint32_t moisture_reading) {
  if (moisture_reading < WATER_THRESHOLD_TENTH_PERCENT) {
    water_me();
  } else {
    all_good();
  }
}


int main(void) {
  int err;
  printf("[Soil Moisture Instructions] When to water\n");

  u8g2_tock_init(&u8g2);

  display_width  = u8g2_GetDisplayWidth(&u8g2);
  display_height = u8g2_GetDisplayHeight(&u8g2);

  u8g2_SetFont(&u8g2, u8g2_font_helvR14_tf);
  u8g2_SetFontPosCenter(&u8g2);

  err = connect_to_sensor_service(ipc_buf, ipc_callback);
  if (err != RETURNCODE_SUCCESS) return -1;

  while (1) yield();
}
