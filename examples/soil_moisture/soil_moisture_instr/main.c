
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <timer.h>
#include <screen.h>
#include <ipc.h>

#include <u8g2-tock.h>
#include <u8g2.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

size_t svc_num = 0;
char ipc_buf[64] __attribute__((aligned(64)));

u8g2_t u8g2;

  int display_width ;
  int display_height;





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

static void all_good(void) {
  u8g2_ClearBuffer(&u8g2);
  u8g2_SendBuffer(&u8g2);
}

static void ipc_callback(__attribute__ ((unused)) int pid,
                           __attribute__ ((unused)) int len,
                           __attribute__ ((unused)) int arg2,
                          __attribute__ ((unused))  void* ud) {

  uint32_t* moisture_buf = (uint32_t*) ipc_buf;
  uint32_t moisture_reading = moisture_buf[0];


    if (moisture_reading < 2000) {
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



  err = ipc_discover("soil_moisture_sensor", &svc_num);
  if (err < 0) {
    printf("No soil moisture service\n");
    return -1;
  }

  err = ipc_register_client_callback(svc_num, ipc_callback, NULL);
  if (err < 0) {
    printf("No ipc_register_client_callback\n");
    return -1;
  }
  err = ipc_share(svc_num, ipc_buf, 64);
  if (err < 0) {
    printf("No ipc_share\n");
    return -1;
  }
  err = ipc_notify_service(svc_num);
  if (err < 0) {
    printf("No ipc_notify_service\n");
    return -1;
  }

  while(1) yield();
}
