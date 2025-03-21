#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/interface/button.h>
#include <libtock/interface/led.h>
#include <libtock/kernel/ipc.h>
#include <libtock/kernel/process_info.h>

// These have to be included before mui.h
#include <u8g2-tock.h>
#include <u8g2.h>

#include <mui.h>
#include <mui_u8g2.h>

u8g2_t u8g2;
mui_t ui;

libtock_alarm_t debounce_alarm;

#define SHARED_BUF_SIZE 512

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
uint16_t binary_selection = 0;
uint8_t process_control = 0;

size_t _app_load_service = -1;
uint8_t _app_load_buf[SHARED_BUF_SIZE] __attribute__((aligned(SHARED_BUF_SIZE)));

uint16_t _number_of_binaries = 0;
bool _done = false;
const char* binary_names[SHARED_BUF_SIZE];

// LOCAL HELPER FUNCTIONS

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

static void set_process_state(uint16_t process_index, uint32_t command) {
  uint32_t count;
  libtock_process_info_get_process_ids(buf, 512, &count);

  if (process_index < count) {
    uint32_t* pids = (uint32_t*) buf;
    uint32_t pid = pids[process_index];
    libtock_process_info_set_process_state(pid, command);
  }
}

// LOCAL MUI FUNCTIONS

// Draw a horizontal line.
static uint8_t mui_hrule(mui_t* mui, uint8_t msg) {
  switch (msg) {
    case MUIF_MSG_DRAW:
      u8g2_DrawHLine(&u8g2, 0, mui_get_y(mui), u8g2_GetDisplayWidth(&u8g2));
      break;
  }
  return 0;
}

// Fill in the app name for the frame header.
static uint8_t mui_u8g2_draw_text_app_name(mui_t *ui_draw, uint8_t msg) {
  char app_name[100];
  get_process_name(selection, app_name);
  snprintf(ui_draw->text, 41, "App: %s", app_name);
  return mui_u8g2_draw_text(ui_draw, msg);
}

// Execute the operation on a given installed process.
uint8_t mui_u8g2_btn_goto_process_control(mui_t *ui, uint8_t msg) {
 if (msg == MUIF_MSG_CURSOR_SELECT) {
      // Need to start/stop/terminate/etc the process.
      uint32_t command = process_control + 1;
      set_process_state(selection, command);
  }

  return mui_u8g2_btn_goto_wm_fi(ui, msg);
}

// Return the number of menu entries for processes running on the board.
uint16_t process_menu_get_item_count(void *data) {
  uint32_t count;
  libtock_process_info_command_get_process_count(&count);
  return count + 1; // +1 for back
}

// Fill in menu entry with the process name (or back).
const char* process_menu_get_item(void* data, uint16_t index) {
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
  } else if (index == count) {
    snprintf(process_names[index], 50, MUI_15 "Back");
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
  return 8;
}

const char *details_get_str(void *data, uint16_t index) {

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
      snprintf(process_names[index], 50, MUI_10 "State Control");
      break;
    }
   case 7: {
      snprintf(process_names[index], 50, MUI_3 "Back");
      break;
    }
  }

  return process_names[index];
}


// For this example, the callback only need set the yield variable.
static void ipc_callback(__attribute__ ((unused)) int   pid,
                         __attribute__ ((unused)) int   len,
                         __attribute__ ((unused)) int   arg2,
                         __attribute__ ((unused)) void* ud) {
  _done = true;
}

// Uses the App Load service to get how many binaries are available.
static void get_number_of_binaries(void) {
  if (_app_load_service == -1) return;

  _app_load_buf[0] = 0;
  _done       = false;
  ipc_notify_service(_app_load_service);
  yield_for(&_done);

  _number_of_binaries = (uint16_t) _app_load_buf[0];
}


// Uses the App Load service to get the names of the binaries.
static void get_binary_names(void) {
  if (_app_load_service == -1) return;

  for (int i = 0; i < _number_of_binaries + 1; i++) {
    if (binary_names[i] == NULL) {
      binary_names[i] = malloc(50);
    }
  }

  _app_load_buf[0] = 1;
  _done       = false;
  ipc_notify_service(_app_load_service);
  yield_for(&_done);

  int offset = 0;

  for (int i = 0; i < _number_of_binaries; i++) {

    size_t binary_name_len = strlen((const char*)&_app_load_buf[offset]);

    if (binary_name_len < 50) {  // Make sure app names are shorter than 50 characters
      strcpy(binary_names[i], (const char*)&_app_load_buf[offset]);
    } else {
      printf("Binary name too long at index %d\n", i);
    }

    offset += binary_name_len + 1;  // 1 for null terminator
  }
  // printf("app name: %s\n", binary_names[i]);
}

// Uses the App Load service to install requested binary.
int install_binary(uint8_t id) {
  if (_app_load_service == -1) return 0;

  _app_load_buf[0] = 2;
  _app_load_buf[1] = id;
  _done       = false;

  ipc_notify_service(_app_load_service);
  yield_for(&_done);

  return  _app_load_buf[0];
}

uint16_t binaries_get_cnt(void *data) {
  get_number_of_binaries();
  return _number_of_binaries + 1;   // one for "Back"
}

const char *binaries_get_str(void *data, uint16_t index) {

  get_binary_names();

  static char* process_names[20] = {NULL};

  if (process_names[index] == NULL) {
    process_names[index] = malloc(50);
    process_names[index][0] = MUI_100;
    process_names[index][1] = '\0';
  }

  snprintf(process_names[index], 50, "%s%s", MUI_21, binary_names[index]);
  if (index == _number_of_binaries){
    snprintf(process_names[index], 50, MUI_15 "Back");
  }

  return process_names[index];

}

static void mui_u8g2_btn_goto_load_new_app(mui_t *ui, uint8_t msg)
{
  if (msg == MUIF_MSG_CURSOR_SELECT) {
    int ret = install_binary(binary_selection);
    if (ret == 0){
        mui_GotoForm(ui, 43, 0);
    }
    else{
      mui_GotoForm(ui, 42, 0);
    }
  }
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

  MUIF_U8G2_U16_LIST("ID", &selection, NULL, process_menu_get_item, process_menu_get_item_count, mui_u8g2_u16_list_goto_w1_pi),
  MUIF_U8G2_U16_LIST("DE", &det_selection, NULL, details_get_str, details_get_cnt, mui_u8g2_u16_list_goto_w1_pi),
  MUIF_U8G2_U16_LIST("LA", &binary_selection, NULL, binaries_get_str, binaries_get_cnt, mui_u8g2_u16_list_goto_w1_pi),

  MUIF_VARIABLE("CM", &process_control, mui_u8g2_u8_opt_line_wa_mse_pi),
  MUIF_BUTTON("CN", mui_u8g2_btn_goto_process_control),

  MUIF_BUTTON("AL", mui_u8g2_btn_goto_load_new_app),
  // MUIF_RO("AL", mui_u8g2_btn_goto_load_new_app),
  // MUIF_BUTTON("NO", mui_u8g2_btn_goto_no_load),


};

fds_t* fds =
  // MAIN HOME SCREEN
  MUI_FORM(15)
  MUI_STYLE(0)
  MUI_LABEL(5, 10, "Process Control")
  MUI_XY("HR", 0, 12)
  MUI_STYLE(1)
  MUI_DATA("GP",
      MUI_3  "Inspect Processes|"
      MUI_20 "Load New Application")
  MUI_XYA("GC", 5, 25, 0)
  MUI_XYA("GC", 5, 37, 1)
  MUI_XYA("GC", 5, 49, 2)







  MUI_FORM(20)
  MUI_STYLE(0)
  MUI_LABEL(5, 10, "Select Application")
  MUI_XY("HR", 0, 12)
  MUI_STYLE(1)
  MUI_XYA("LA", 5, 25, 0)
  MUI_XYA("LA", 5, 37, 1)
  MUI_XYA("LA", 5, 49, 2)
  MUI_XYA("LA", 5, 61, 3)

  MUI_FORM(21)
  // MUI_AUX("AL")
  MUI_STYLE(0)
  MUI_LABEL(5, 10, "Load Application?")
  // &u8g2.drawButtonUTF8(62, 20, U8G2_BTN_HCENTER, 34,  2,  2, "Yes" );
  // MUI_XYAT("AL", 45, 25, 24, "Yes")
  MUI_XYAT(".G", 45, 35, 40, "Yes")
  MUI_XYAT(".G", 55, 48, 20, "No")
  // MUI_XYT("NO", 65, 38, "No")


  MUI_FORM(40)
  MUI_AUX("AL")
  MUI_STYLE(0)
  MUI_LABEL(5, 10, "Loading App...")

  MUI_FORM(42)
  MUI_STYLE(0)
  MUI_LABEL(5, 10, "Loading Failed!")
  MUI_STYLE(1)
  MUI_XYAT(".G", 46, 25, 20, "Back")


  MUI_FORM(43)
  MUI_STYLE(0)
  MUI_LABEL(5, 10, "Loading Success!")
  MUI_STYLE(1)
  MUI_XYAT(".G", 46, 25, 20, "Back")


  MUI_FORM(3)
  MUI_STYLE(0)
  MUI_LABEL(5, 10, "Process List")
  MUI_XY("HR", 0, 12)
  MUI_STYLE(1)
  MUI_XYA("ID", 5, 25, 0)
  MUI_XYA("ID", 5, 37, 1)
  MUI_XYA("ID", 5, 49, 2)
  // MUI_XYA("ID", 5, 61, 3)


  MUI_FORM(4)
  MUI_STYLE(0)
  MUI_XYT("AT", 5, 10, "APP")
  MUI_XY("HR", 0, 12)
  MUI_STYLE(1)
  MUI_XYA("DE", 5, 25, 0)
  MUI_XYA("DE", 5, 37, 1)
  MUI_XYA("DE", 5, 49, 2)
  // MUI_XYA("DE", 5, 61, 3)

  MUI_FORM(10)
  MUI_STYLE(0)
  MUI_LABEL(5, 10, "Change State")
  MUI_XY("HR", 0, 12)
  MUI_STYLE(1)
  MUI_XYT("AT", 5, 25, "APP")
  MUI_XYAT("CM", 15, 45, 60, "Start|Stop|Fault|Terminate|Boot")
  MUI_STYLE(0)
  MUI_XYAT("CN", 100, 45, 4, "OK")
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
  libtock_alarm_in_ms(200, debounce_cb, NULL, &debounce_alarm);
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
  if (ret != RETURNCODE_SUCCESS) return ret;

  for (int i = 0; i < count; i++) {
    libtock_button_notify_on_press(i, button_callback);
  }

  ret = ipc_discover("app_loader", &_app_load_service);
  if (ret != RETURNCODE_SUCCESS) {
    printf("No App Loader Service\n");
  } else {
    printf("Discovered App Loader Service\n");
  }

  // Setup IPC for App Loader service
  if (_app_load_service > -1) {
    ipc_register_client_callback(_app_load_service, ipc_callback, NULL);
    ipc_share(_app_load_service, _app_load_buf, SHARED_BUF_SIZE);
  }

  // Setup the screen.
  ret = u8g2_tock_init(&u8g2);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Process Control] Unable to access screen.\n");
    return -1;
  }

  // Main MUI screen drawing loop.
  u8g2_ClearBuffer(&u8g2);

  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);

  mui_Init(&ui, &u8g2, fds, muif_list, sizeof(muif_list) / sizeof(muif_t));
  mui_GotoForm(&ui, 15, 0);

  while (1) {

    u8g2_FirstPage(&u8g2);
    do {
      mui_Draw(&ui);
    } while (u8g2_NextPage(&u8g2));

    action = false;
    yield_for(&action);
  }
}