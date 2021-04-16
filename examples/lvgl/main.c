#include "lvgl_driver.h"
#include <stdio.h>
#include <timer.h>

int main (void)
{
  screen_set_brightness (100);
  int status = lvgl_driver_init (5);
  if (status == RETURNCODE_SUCCESS) {
    /* LittlevGL's Hello World tutorial example */

    lv_obj_t * scr = lv_disp_get_scr_act(NULL);         /*Get the current screen*/

    /*Create a Label on the currently active screen*/
    lv_obj_t * label1 =  lv_label_create(scr, NULL);

    /*Modify the Label's text*/
    lv_label_set_text(label1, "Hello world!");

    /* Align the Label to the center
     * NULL means align on parent (which is the screen now)
     * 0, 0 at the end means an x, y offset after alignment*/
    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);

    unsigned int t = 0;
    /* main loop */
    while (1)
    {
      t++;
      if (t % 200 == 0) {
        char buffer[100];
        snprintf (buffer, 99, "Seconds: %d", t / 200);
        lv_label_set_text(label1, buffer);
      }
      delay_ms (5);
      lvgl_driver_event (5);
    }
  } else {
    printf ("lvgl init error: %s\n", tock_strrcode(status));
  }
  return 0;
}

