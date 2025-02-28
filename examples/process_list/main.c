#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/interface/button.h>
#include <libtock/interface/led.h>

// These have to be included before mui.h
#include <u8g2-tock.h>
#include <u8g2.h>

#include <mui.h>
#include <mui_u8g2.h>

u8g2_t u8g2;
mui_t ui;

libtock_alarm_t debounce_alarm;

bool action = false;

uint8_t my_value    = 2;
uint8_t my_value2   = 2;
uint8_t my_value3   = 0;
uint8_t my_color    = 0;
uint8_t my_color2    = 0;
uint8_t fruit_input = 0;

uint8_t touchscreen_selection = 255;

uint8_t checkbox_led0 = 0;
uint8_t checkbox_led1 = 0;
uint8_t checkbox_led2 = 0;
uint8_t checkbox_led3 = 0;

uint8_t buf[512];
uint8_t buf1[512];

uint16_t selection = 0;
uint16_t det_selection = 0;

static void get_process_name(uint16_t process_index, char* name) {
  uint32_t count;
  libtock_process_info_get_process_ids(buf, 512, &count);

  if (process_index < count) {
    uint32_t* pids = (uint32_t*) buf;
    uint32_t pid = pids[process_index];
    libtock_process_info_get_process_name(pid, buf1, 512);
    strcpy(name, buf1);
  }
}

// local mui functions

static uint8_t mui_hrule(mui_t* mui, uint8_t msg) {
  switch (msg) {
    case MUIF_MSG_DRAW:
      u8g2_DrawHLine(&u8g2, 0, mui_get_y(mui), u8g2_GetDisplayWidth(&u8g2));
      break;
  }
  return 0;
}


static uint8_t mui_u8g2_draw_text_app_name(mui_t *ui_draw, uint8_t msg) {
  char app_name[100];
  get_process_name(selection, app_name);
  snprintf(ui_draw->text, 41, "App: %s", app_name);
  return mui_u8g2_draw_text(ui_draw, msg);
}




uint16_t menu_get_cnt(void *data) {
  uint32_t count;
  libtock_process_info_command_get_process_count(&count);
  return count;
}

const char *menu_get_str(void *data, uint16_t index) {
  static char* process_names[20] = {NULL};

  if (process_names[index] == NULL) {
    process_names[index] = malloc(50);
    process_names[index][0] = MUI_100;
    process_names[index][1] = '\0';
  }

  uint32_t count;
  libtock_process_info_get_process_ids(buf, 512, &count);

  if (index < count) {

    uint32_t* pids = (uint32_t*) buf;


    libtock_process_info_get_process_name(pids[index], buf1, 512);

    snprintf(process_names[index], 50, MUI_4 "%s", buf1);
  }

  return process_names[index];


}

static uint32_t get_stat(uint16_t process_index, uint16_t stat_index) {
  uint32_t count;
  libtock_process_info_get_process_ids(buf, 512, &count);
  uint32_t* pids = (uint32_t*) buf;
  uint32_t pid = pids[process_index];
  libtock_process_info_get_process_stats(pid, buf1, 512);
  uint32_t* stats = (uint32_t*) buf1;
  return stats[stat_index];
}


uint16_t details_get_cnt(void *data) {
  return 7;
}

const char *details_get_str(void *data, uint16_t index) {
  // static const char *menu[] = {
  //   MUI_10 "PID: 5",
  //   MUI_11 "Address",
  //   MUI_3  "Goto Main Menu",
  // };
  // return menu[index];


  static char* process_names[20] = {NULL};



  if (process_names[index] == NULL) {
    process_names[index] = malloc(50);
    process_names[index][0] = MUI_100;
    process_names[index][1] = '\0';
  }

  switch (index) {
   case 0: {
    uint32_t count;
    libtock_process_info_get_process_ids(buf, 512, &count);

    uint32_t* pids = (uint32_t*) buf;
    uint32_t pid = pids[selection];



      snprintf(process_names[index], 50, MUI_100 "PID: %d", pid);
      break;
    }
   case 1: {
    uint32_t count;
    libtock_process_info_get_short_ids(buf, 512, &count);

    uint32_t* shortids = (uint32_t*) buf;
    uint32_t shortid = shortids[selection];

    if (shortid == 0) {
            snprintf(process_names[index], 50, MUI_100 "ShortID: Unique");

    } else {
      snprintf(process_names[index], 50, MUI_100 "ShortID: %#02x", shortid);
    }
      break;
   }
   case 2: {
    uint32_t timeslices_expired = get_stat(selection, 0);
    snprintf(process_names[index], 50, MUI_100 "Timeslices Exp: %d", timeslices_expired);
    break;
   }
   case 3: {
    uint32_t syscall_count = get_stat(selection, 1);
    snprintf(process_names[index], 50, MUI_100 "Syscall Count: %d", syscall_count);
    break;
   }
   case 4: {
    uint32_t restart_count = get_stat(selection, 2);
    snprintf(process_names[index], 50, MUI_100 "Restart Count: %d", restart_count);
    break;
   }
   case 5: {
    char* states[] = {
      "Running",
      "Yielded",
      "YieldedFor",
      "Stopped",
      "Faulted",
      "Terminated",
    };
    uint32_t state = get_stat(selection, 3);
    snprintf(process_names[index], 50, MUI_100 "State: %s", states[state]);
    break;
   }
   case 6: {
      snprintf(process_names[index], 50, MUI_3 "Back");
      break;
    }
  }

  return process_names[index];
}






muif_t muif_list[] = {
  // normal text style
  MUIF_U8G2_LABEL(),
  MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvB08_tr),
  MUIF_U8G2_FONT_STYLE(1, u8g2_font_helvR08_tr),

  MUIF_RO("HR", mui_hrule),

  MUIF_RO("GP", mui_u8g2_goto_data),
  MUIF_BUTTON("GC", mui_u8g2_goto_form_w1_pi),

  MUIF("I2", MUIF_CFLAG_IS_CURSOR_SELECTABLE, &my_value2, mui_u8g2_u8_opt_line_wa_mse_pi),
  MUIF("I3", MUIF_CFLAG_IS_CURSOR_SELECTABLE, &my_value3, mui_u8g2_u8_chkbox_wm_pi),

  MUIF("O4", MUIF_CFLAG_IS_CURSOR_SELECTABLE, &my_color, mui_u8g2_u8_opt_child_wm_pi),

  MUIF("AT", 0, &my_color2, mui_u8g2_draw_text_app_name),

  MUIF_VARIABLE("O1", &fruit_input, mui_u8g2_u8_opt_line_wa_mse_pi),

  MUIF_VARIABLE("C0", &checkbox_led0, mui_u8g2_u8_chkbox_wm_pi),
  MUIF_VARIABLE("C1", &checkbox_led1, mui_u8g2_u8_chkbox_wm_pi),
  MUIF_VARIABLE("C2", &checkbox_led2, mui_u8g2_u8_chkbox_wm_pi),
  MUIF_VARIABLE("C3", &checkbox_led3, mui_u8g2_u8_chkbox_wm_pi),

  MUIF("TS", MUIF_CFLAG_IS_CURSOR_SELECTABLE, &touchscreen_selection, mui_u8g2_u8_opt_line_wa_mse_pi),
  MUIF(".G", MUIF_CFLAG_IS_CURSOR_SELECTABLE, 0, mui_u8g2_btn_goto_w2_fi),    /* MUI_GOTO has the id FG */
  MUIF(".g", MUIF_CFLAG_IS_CURSOR_SELECTABLE, 0, mui_u8g2_btn_goto_w1_pi),    /* MUI_goto has the id Fg */
  MUIF(".L", 0, 0, mui_u8g2_draw_text),

  MUIF_U8G2_U16_LIST("ID", &selection, NULL, menu_get_str, menu_get_cnt, mui_u8g2_u16_list_goto_w1_pi),
  MUIF_U8G2_U16_LIST("DE", &det_selection, NULL, details_get_str, details_get_cnt, mui_u8g2_u16_list_goto_w1_pi),

};

fds_t* fds =
  MUI_FORM(3)
  MUI_STYLE(0)
  MUI_LABEL(12, 10, "Process List")
  MUI_STYLE(1)
  MUI_XYA("ID", 5, 25, 0)
  MUI_XYA("ID", 5, 37, 1)
  MUI_XYA("ID", 5, 49, 2)
  MUI_XYA("ID", 5, 61, 3)

  MUI_FORM(4)
  MUI_STYLE(0)
  // MUI_LABEL(12, 10, "Process Details")
  MUI_XYT("AT", 5, 10, "APP")
  MUI_STYLE(1)
  MUI_XYA("DE", 5, 25, 0)
  MUI_XYA("DE", 5, 37, 1)
  MUI_XYA("DE", 5, 49, 2)
  MUI_XYA("DE", 5, 61, 3)



//   MUI_FORM(1)
//   MUI_STYLE(0)
//   MUI_LABEL(12, 10, "TOCK MENU")
//   MUI_XY("HR", 0, 12)
//   MUI_STYLE(1)
//   MUI_DATA("GP",
//            MUI_2 "Fruit Select|"
//            MUI_3 "LED Control|"
//            MUI_2 "unused0|"
//            MUI_2 "unused1|"
//            MUI_2 "unused2|"
//            MUI_2 "unused3|"
//            MUI_2 "unused4")
//   MUI_XYA("GC", 5, 25, 0)
//   MUI_XYA("GC", 5, 37, 1)
//   MUI_XYA("GC", 5, 49, 2)
//   MUI_XYA("GC", 5, 61, 3)

//   MUI_FORM(2)
//   MUI_STYLE(0)
//   MUI_LABEL(5, 10, "Select Your Fruit")
//   MUI_XY("HR", 0, 12)
//   MUI_STYLE(1)
//   MUI_LABEL(5, 25, "Fruit:")
//   MUI_XYAT("O1", 60, 25, 60, "Banana|Apple|Melon|Cranberry")
//   MUI_STYLE(0)
//   MUI_GOTO(64, 59, 1, " Ok ")

// // LED selection page
//   MUI_FORM(4)
//   MUI_STYLE(0)
//   MUI_LABEL(5, 10, "LED Control Menu")
//   MUI_XY("HR", 0, 12)
//   MUI_STYLE(1)
//   MUI_LABEL(5, 25, "LED 1:")
//   MUI_XY("C0", 40, 25)
//   MUI_LABEL(60, 25, "LED 2:")
//   MUI_XY("C1", 95, 25)
//   MUI_LABEL(5, 37, "LED 3:")
//   MUI_XY("C2", 40, 37)
//   MUI_LABEL(60, 37, "LED 4:")
//   MUI_XY("C3", 95, 37)
//   MUI_STYLE(0)
//   MUI_GOTO(64, 59, 1, " Ok ")
;

struct alarm_cb_data {
  bool debouncing;
};

static struct alarm_cb_data data = { .debouncing = false };

static void debounce_cb(__attribute__ ((unused)) uint32_t now,
                        __attribute__ ((unused)) uint32_t scheduled,
                        __attribute__ ((unused)) void*    opaque) {
  data.debouncing = false;
}

static void start_debounce(void) {
  data.debouncing = true;
  libtock_alarm_in_ms(300, debounce_cb, NULL, &debounce_alarm);
}

static void button_callback(
  __attribute__ ((unused)) returncode_t ret,
  int                                   btn_num,
  bool                                  val) {
  if (data.debouncing) return;
  start_debounce();

  if (val) {
    if (btn_num == 0) {
      mui_PrevField(&ui);
    } else if (btn_num == 2) {
      mui_NextField(&ui);
    } else if (btn_num == 3) {
      mui_SendSelect(&ui);
    }

    action = true;
  }
}



int main(void) {
  returncode_t ret;

  // Enable interrupts on each button.
  int count;
  ret = libtock_button_count(&count);
  if (ret < 0) return ret;

  for (int i = 0; i < count; i++) {
    libtock_button_notify_on_press(i, button_callback);
  }

  ret = u8g2_tock_init(&u8g2);

  u8g2_ClearBuffer(&u8g2);

  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);

  mui_Init(&ui, &u8g2, fds, muif_list, sizeof(muif_list) / sizeof(muif_t));
  mui_GotoForm(&ui, 3, 0);

  while (1) {

    u8g2_FirstPage(&u8g2);
    do
    {
      mui_Draw(&ui);
    } while (u8g2_NextPage(&u8g2));

    action = false;
    yield_for(&action);
    // printf("fruit %i\n", fruit_input);

    // if (checkbox_led0) {
    //   libtock_led_on(0);
    // } else {
    //   libtock_led_off(0);
    // }
    // if (checkbox_led1) {
    //   libtock_led_on(1);
    // } else {
    //   libtock_led_off(1);
    // }
    // if (checkbox_led2) {
    //   libtock_led_on(2);
    // } else {
    //   libtock_led_off(2);
    // }
    // if (checkbox_led3) {
    //   libtock_led_on(3);
    // } else {
    //   libtock_led_off(3);
    // }

  }
}
