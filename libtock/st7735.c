#include "st7735.h"
#include <stdio.h>
#include <tock.h>

static void st7735_callback(__attribute__ ((unused)) int value,
                            __attribute__ ((unused)) int unused1,
                            __attribute__ ((unused)) int unused2,
                            void* ud) {
  *(bool*)ud = true;
}

static int st7735_subscribe (subscribe_cb cb, void *userdata) {
  return subscribe(DRIVER_NUM_ST7735, 0, cb, userdata);
}

static int st7735_command (int command_num, int data1, int data2) {
  return command(DRIVER_NUM_ST7735, command_num, data1, data2);
}

int st7735_init (void)
{
  bool done = false;
  st7735_subscribe (st7735_callback, &done);
  int v = st7735_command (1, 0, 0);
  if (v == TOCK_SUCCESS) yield_for (&done);
  st7735_subscribe (NULL, NULL);
  return v;
}

int st7735_fill (int color)
{
  bool done = false;
  st7735_subscribe (st7735_callback, &done);
  int v = st7735_command (2, color, 0);
  if (v == TOCK_SUCCESS) yield_for (&done);
  st7735_subscribe (NULL, NULL);
  return v;
}