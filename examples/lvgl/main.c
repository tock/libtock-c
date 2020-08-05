#include "lvgl_driver.h"
#include <stdio.h>
#include <timer.h>
#include <adc.h>

lv_obj_t * label1 = NULL;

static void event_handler(lv_obj_t * obj __attribute__((unused)), lv_event_t event)
{
  if (event == LV_EVENT_CLICKED) {
    printf("Clicked\n");
    uint16_t data;
    adc_sample_sync (0, &data);
    
    char label[100];

    snprintf (label, 23, "Potentiometer: %d", data);

    /*Modify the Label's text*/
    lv_label_set_text(label1, label);
  }else if (event == LV_EVENT_VALUE_CHANGED)   {
    printf("Toggled\n");
  }
}

static void lv_ex_btn_1(void)
{
  lv_obj_t * label;

  lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), NULL);
  lv_obj_set_event_cb(btn1, event_handler);
  lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, -40);

  label = lv_label_create(btn1, NULL);
  lv_label_set_text(label, "Button");

  lv_obj_t * btn2 = lv_btn_create(lv_scr_act(), NULL);
  lv_obj_set_event_cb(btn2, event_handler);
  lv_obj_align(btn2, NULL, LV_ALIGN_CENTER, 0, 40);
  lv_btn_set_toggle(btn2, true);
  lv_btn_toggle(btn2);
  lv_btn_set_fit2(btn2, LV_FIT_NONE, LV_FIT_TIGHT);

  label = lv_label_create(btn2, NULL);
  lv_label_set_text(label, "Toggled");
}

int main (void)
{
  screen_set_brightness (100);
  int status = lvgl_driver_init (5);
  if (status == TOCK_SUCCESS) {
    /* LittlevGL's Hello World tutorial example */

    lv_obj_t * scr = lv_disp_get_scr_act(NULL);         /*Get the current screen*/

    /*Create a Label on the currently active screen*/
    label1 =  lv_label_create(scr, NULL);

    /*Modify the Label's text*/
    lv_label_set_text(label1, "Hello world!");

    /* Align the Label to the center
     * NULL means align on parent (which is the screen now)
     * 0, 0 at the end means an x, y offset after alignment*/
    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);

    lv_ex_btn_1();

    /* main loop */
    while (1)
    {
      delay_ms (5);
      lvgl_driver_event (5);
    }
  } else {
    printf ("lvgl init error: %s\n", tock_strerror(status));
  }
  return 0;
}

