#include <screen.h>
#include <stdio.h>
#include <stdlib.h>

#include <screen.h>
#include <button.h>
#include <timer.h>

#include <u8g2-tock.h>
#include <u8g2.h>
#include <mui.h>
#include <mui_u8g2.h>

u8g2_t u8g2;
mui_t ui;

bool action = false;

uint8_t my_value = 2;
uint8_t my_value2 = 2;
uint8_t my_value3 = 0;
uint8_t my_color = 0;

uint8_t touchscreen_selection = 255;

/*

  field function naming convention

    action
      draw_text:                        (rename from draw label)
      draw_str:
      btn_jmp   button jump to:                      a button which jumps to a specific form
      btn_exit          button leave:                     a button which leaves the form and places an exit code into a uint8 variable
      u8_value_0_9
      u8_chkbox
      u8_radio
      u8_opt_line       edit value options in the same line
      u8_opt_parent       edit value options parent
      u8_opt_child       edit value options child


    field width (not for draw text/str)
      wm                minimum width
      wa                width can be provided via FDS argument
      w1                full display width
      w2                half display size (minus some pixel)
      w3                one/third of the dispay width (minus some pixel)

    edit mode  (not for draw text/str, buttons and checkbox)
      mse       select: select event will increment the value or activate the field (buttons)
      mud      up/down:  select will enter the up/down edit mode. Next/prev event will increment/decrement the value

    styles (not for draw text/str)
      unselected                selected            up/down edit               postfix         Use for
      plain                     invers              invers + gap + frame       pi              input elements
      frame                     invers+frame        frame                      fi              buttons

      plain                     frame               invers + frame             pf              input elements
      invers                    frame               invers + frame             if              buttons


    mui_u8g2_[action]_[field_width]_[edit_mode]_[style]
    */


muif_t muif_list[] = {
  // // normal text style
  // MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvR08_tr),


  // MUIF("B0",MUIF_CFLAG_IS_CURSOR_SELECTABLE,0,mui_half_width_frame_button_invers_select_u8g2),
  // MUIF("B1",MUIF_CFLAG_IS_CURSOR_SELECTABLE,0,mui_half_width_frame_button_invers_select_u8g2),
  // MUIF("I1",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&my_value,mui_u8g2_u8_value_0_9_wm_mse_pi),
  MUIF("I2",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&my_value2,mui_u8g2_u8_opt_line_wa_mse_pi),
  MUIF("I3",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&my_value3,mui_u8g2_u8_chkbox_wm_pi),
  // MUIF("I4",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&my_color,mui_u8g2_u8_opt_parent_wm_pi),

  MUIF("O4",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&my_color,mui_u8g2_u8_opt_child_wm_pi),

  MUIF("TS",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&touchscreen_selection,mui_u8g2_u8_opt_line_wa_mse_pi),
  MUIF(".G",MUIF_CFLAG_IS_CURSOR_SELECTABLE,0,mui_u8g2_btn_goto_w2_fi),    /* MUI_GOTO has the id FG */
  MUIF(".g",MUIF_CFLAG_IS_CURSOR_SELECTABLE,0,mui_u8g2_btn_goto_w1_pi),    /* MUI_goto has the id Fg */
  MUIF(".L",0,0,mui_u8g2_draw_text)


};

fds_t *fds =

MUI_FORM(1)
MUI_LABEL(12,10, "MENU")
MUI_goto(12,22,2, "Choice 1")
MUI_goto(12,34,2, "Choice 2")

MUI_FORM(2)
MUI_STYLE(0)
MUI_LABEL(5,40, "Fruit:")
MUI_XYAT("ID",60, 40, 60, "Banana|Apple|Melon|Cranberry")
MUI_STYLE(0)
MUI_GOTO(64, 59, 1, " Ok ")

;

static void button_callback(int                            btn_num,
                            int                            val,
                            __attribute__ ((unused)) int   arg2,
                            __attribute__ ((unused)) void *ud) {
  if (val == 1) {
    printf("b\n");
    mui_NextField(&ui);
    action = true;
  }
}

int main(void) {
  returncode_t ret;


  ret = button_subscribe(button_callback, NULL);
  if (ret < 0) return ret;

  // Enable interrupts on each button.
  int count;
  ret = button_count(&count);
  if (ret < 0) return ret;

  for (int i = 0; i < count; i++) {
    button_enable_interrupt(i);
  }


  ret = u8g2_tock_init(&u8g2);


  u8g2_ClearBuffer(&u8g2);


  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));

  mui_Init(&ui, &u8g2, fds, muif_list, sizeof(muif_list)/sizeof(muif_t));
  mui_GotoForm(&ui, 1, 0);

  while(1) {

    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);

      // u8g2_FirstPage(&u8g2);
      mui_Draw(&ui);

    } while( u8g2_NextPage(&u8g2) );

    // int width  = u8g2_GetDisplayWidth(&u8g2);
    // int height = u8g2_GetDisplayHeight(&u8g2);

    // int center_x = width / 2;

    // u8g2_DrawTriangle(&u8g2, center_x, 0, 0, height - 1, width - 1, height - 1);

    // u8g2_SendBuffer(&u8g2);
    //
    //
    action = false;
    while (1) {
     yield_for(&action);
   }
  }
}
