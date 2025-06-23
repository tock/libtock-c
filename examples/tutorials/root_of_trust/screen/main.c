#include <stdio.h>
#include <stdlib.h>

#include <libtock/interface/button.h>
#include <libtock/kernel/ipc.h>

#include <u8g2-tock.h>
#include <u8g2.h>

#include <mui.h>
#include <mui_u8g2.h>

#define MIN(i, j) (((i) < (j)) ? (i) : (j))

#define NO_SELECTED_SERVICE  0
#define ENCRYPTION_SERVICE   1
#define SUSPICIOUS_SERVICE   2
#define QUESTIONABLE_SERVICE 3

#define LOG_NUM_LINES   6
#define LOG_WIDTH       32
#define LOG_LINE_HEIGHT 10

const char ENCRYPTION_SERVICE_NAME[]   = "org.tockos.tutorials.attestation.encryption";
const char SUSPICIOUS_SERVICE_NAME[]   = "org.tockos.tutorials.attestation.suspicious";
const char QUESTIONABLE_SERVICE_NAME[] = "org.tockos.tutorials.attestation.questionable";

u8g2_t u8g2;
mui_t ui;

bool action              = false;
uint8_t app_choice       = 0;
size_t app_start_service = -1;

uint8_t log_head = 0;
uint8_t log_size = 0;
uint8_t log_buf[LOG_NUM_LINES][LOG_WIDTH];

static uint8_t mui_hrule(mui_t* mui, uint8_t msg) {
  switch (msg) {
    case MUIF_MSG_DRAW:
      u8g2_DrawHLine(&u8g2, 0, mui_get_y(mui), u8g2_GetDisplayWidth(&u8g2));
      break;
  }
  return 0;
}

muif_t muif_list[] = {
  MUIF_U8G2_LABEL(),
  MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvB08_tr),
  MUIF_U8G2_FONT_STYLE(1, u8g2_font_squeezed_r6_tr),

  MUIF_RO("HR", mui_hrule),
  MUIF_VARIABLE("RB", &app_choice, mui_u8g2_u8_radio_wm_pi),
  MUIF_VARIABLE("ST", NULL, mui_u8g2_btn_exit_wm_fi),
};

fds_t* fds =
  MUI_FORM(1)
  MUI_STYLE(0)
  MUI_LABEL(18, 10, "Tock HWRoT Demo")
  MUI_XY("HR", 0, 12)
  MUI_STYLE(1)
  MUI_LABEL(5, 25, "Service: ")
  MUI_XYAT("RB", 40, 25, 1, "Encryption service")
  MUI_XYAT("RB", 40, 35, 2, "Suspicious service")
  MUI_XYAT("RB", 40, 45, 3, "Questionable service")
  MUI_XYT("ST", 63, 58, " Start ")
;

static void button_callback(
  __attribute__ ((unused)) returncode_t ret,
  int                                   btn_num,
  bool                                  val) {

  // Handle button presses, routing them to the menu UI library.
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

static void ipc_callback(int                            pid,
                         int                            len,
                         int                            buf,
                         __attribute__ ((unused)) void* ud) {

  // Add message at the head of the log ring buffer.
  memcpy(log_buf[log_head], (char*)buf, MIN(LOG_WIDTH - 1, len));
  log_buf[log_head][LOG_WIDTH - 1] = '\0';

  // Move the ring buffer head along to the next line to write.
  log_head = (log_head + 1) % LOG_NUM_LINES;

  // Update the size of the ring buffer, so we print starting
  // at the top of the screen.
  if (log_size < LOG_NUM_LINES) log_size += 1;

  // Clear the screen and use smaller font.
  u8g2_ClearBuffer(&u8g2);
  u8g2_SetFont(&u8g2, u8g2_font_squeezed_r6_tr);

  // Compute the index of the first log line to print (the "tail").
  uint8_t log_tail = ((log_head - log_size) + LOG_NUM_LINES) % LOG_NUM_LINES;

  // Draw the log entries on the screen, starting from the tail.
  for (uint8_t i = 0; i < log_size; i++) {
    uint8_t log_index = (log_tail + i) % LOG_NUM_LINES;
    u8g2_DrawStr(&u8g2, 5, LOG_LINE_HEIGHT + i * LOG_LINE_HEIGHT, (const char*)log_buf[log_index]);
  }

  // Send the results.
  u8g2_SendBuffer(&u8g2);
  ipc_notify_client(pid);
}

static int enable_interrupts(void) {
  returncode_t ret;

  // Enable interrupts on each button.
  int count;
  ret = libtock_button_count(&count);
  if (ret != RETURNCODE_SUCCESS) {
    printf("Unable to fetch button count.\n");
    return ret;
  }

  // Register a shared callback for every button.
  for (int i = 0; i < count; i++) {
    libtock_button_notify_on_press(i, button_callback);
  }

  return 0;
}

static int prompt_for_service_choice(void) {
  returncode_t ret;

  // Initialize the menu.
  ret = u8g2_tock_init(&u8g2);
  if (ret < 0) {
    printf("Failed to initialize graphics library.\n");
    return ret;
  }

  u8g2_ClearBuffer(&u8g2);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);

  mui_Init(&ui, &u8g2, fds, muif_list, sizeof(muif_list) / sizeof(muif_t));
  mui_GotoForm(&ui, 1, 0);

  // Redraw on each button press until a selection is made.
  while (mui_IsFormActive(&ui)) {
    u8g2_FirstPage(&u8g2);
    do
    {
      mui_Draw(&ui);
    } while (u8g2_NextPage(&u8g2));

    action = false;
    yield_for(&action);
  }

  return 0;
}

static int select_rot_service(const char* service) {
  returncode_t ret;

  // Discover the provided encryption app IPC service.
  ret = ipc_discover(service, &app_start_service);
  if (ret != RETURNCODE_SUCCESS) {
    printf("Root of trust service not found.\n");
    return ret;
  }

  // Register a callback so that the app can log to screen.
  ipc_register_service_callback("org.tockos.tutorials.attestation.screen", ipc_callback,
                                NULL);

  // Notify it so the correct encryption app can start.
  ret = ipc_notify_service(app_start_service);
  if (ret != RETURNCODE_SUCCESS) {
    printf("Encryption oracle service unable to be notified.\n");
    return ret;
  }

  return 0;
}

int main(void) {
  returncode_t ret;

  // Enable interrupts so button presses register.
  ret = enable_interrupts();
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Prompt the user for an app until they select one.
  while (app_choice == NO_SELECTED_SERVICE) {
    ret = prompt_for_service_choice();
    if (ret != RETURNCODE_SUCCESS) return ret;
  }

  // Clear the screen so that the app has a fresh start.
  u8g2_ClearBuffer(&u8g2);

  // Based on the app choice, notify the proper app over IPC.
  switch (app_choice) {
    case ENCRYPTION_SERVICE:
      printf("Starting encryption service...\n");
      ret = select_rot_service(ENCRYPTION_SERVICE_NAME);
      break;
    case SUSPICIOUS_SERVICE:
      printf("Starting suspicious service...\n");
      ret = select_rot_service(SUSPICIOUS_SERVICE_NAME);
      break;
    case QUESTIONABLE_SERVICE:
      printf("Starting questionable service...\n");
      ret = select_rot_service(QUESTIONABLE_SERVICE_NAME);
      break;
  }
  while (1) {
    yield();
  }
}
