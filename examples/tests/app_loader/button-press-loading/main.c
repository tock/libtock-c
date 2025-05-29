#include <math.h>
#include <stdio.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/interface/button.h>
#include <libtock/kernel/app_loader.h>
#include <libtock/tock.h>

#define FLASH_BUFFER_SIZE 512
#define RETURNCODE_SUCCESS 0

static bool setup_done    = false;   // to check if setup is done
static bool write_done    = false;   // to check if writing to flash is done
static bool finalize_done = false;   // to check if the kernel is done finalizing the process binary
static bool load_done     = false;   // to check if the process was loaded successfully
static bool app_load      = false;   // to check if there is a request to load a new app

// Variables to hold loadable app information
size_t app_size;
const char*    app_name;
const uint8_t* app_data;

/******************************************************************************************************
* Loadable Applications
******************************************************************************************************/

// LEGACY COMPILER SUPPORT
// Avoid #embed until it's more ubiqutious (req's gcc15+, clang19+)
//
// #ifdef __has_embed
// static const uint8_t APP_BLINK[] = {
// #embed "blink.embed"
// };
// static const uint8_t APP_ADC[] = {
// #embed "adc.embed"
// };
// #else
#include "blink.xxd"
#define APP_BLINK blink_embed
#include "adc.xxd"
#define APP_ADC adc_embed
// #endif


/******************************************************************************************************
* Callback functions
*
* 1. Callback to let us know when the capsule is done writing data to flash
* 2. Set button callback to initiate the dynamic app load process on pressing button 1 (on nrf52840dk)
*
******************************************************************************************************/

static void app_setup_done_callback(__attribute__((unused)) int   arg0,
                                    __attribute__((unused)) int   arg1,
                                    __attribute__((unused)) int   arg2,
                                    __attribute__((unused)) void* ud) {
  setup_done = true;
}

static void app_write_done_callback(__attribute__((unused)) int   arg0,
                                    __attribute__((unused)) int   arg1,
                                    __attribute__((unused)) int   arg2,
                                    __attribute__((unused)) void* ud) {
  write_done = true;
}

static void app_finalize_done_callback(__attribute__((unused)) int   arg0,
                                       __attribute__((unused)) int   arg1,
                                       __attribute__((unused)) int   arg2,
                                       __attribute__((unused)) void* ud) {
  finalize_done = true;
}

static void app_load_done_callback(int                           arg0,
                                   __attribute__((unused)) int   arg1,
                                   __attribute__((unused)) int   arg2,
                                   __attribute__((unused)) void* ud) {

  if (arg0 != RETURNCODE_SUCCESS) {
    printf("[Error] Process creation failed: %d.\n", arg0);
  } else {
    printf("[Success] Process created successfully.\n");
  }
  load_done = true;
}

// Callback for button presses.
static void button_callback(__attribute__ ((unused)) returncode_t retval, int btn_num, bool pressed) {
  // Callback for button presses.
  // val: 1 if pressed, 0 if depressed
  if (pressed == 1 && !app_load) {
    // Note: this variable is introduced
    // because the current alarm upcall implementation
    // results in panic when a button is pressed within
    // the debounce period if the debounce period is long.
    // Setting the debounce interval to 100ms seems to work
    // but setting it to 200ms and rapidly clicking buttons
    // leads to the kernel panicking.
    app_load = true;
    libtocksync_alarm_delay_ms(200); // debounce

    if (pressed == 1) {
      app_name = NULL;
      app_data = NULL;
      app_size = 0;

      switch (btn_num) {
        case BUTTON1:
          app_name = "blink";
          app_data = APP_BLINK;
          app_size = sizeof(APP_BLINK);
          break;
        case BUTTON2:
          app_name = "adc";
          app_data = APP_ADC;
          app_size = sizeof(APP_ADC);
          break;
        default:
          printf("[Log] Unsupported Button.\n");
          app_load = false;
          return;
      }
    }
  }
}


/******************************************************************************************************
* Helper Function for the apploader machine
*
* Takes app size and the app binary as arguments
******************************************************************************************************/

static void appload(double size, const uint8_t binary[]) {
  int ret = libtock_app_loader_setup(size);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Error] Setup Failed: %d.\n", ret);
    tock_exit(ret);
  }

  // wait on setup done callback
  yield_for(&setup_done);
  setup_done = false;

  printf("[Success] Setup successful. Writing app to flash.\n");
  int ret1 = write_app(size, binary);
  if (ret1 != RETURNCODE_SUCCESS) {
    printf("[Error] App flash write unsuccessful: %d.\n", ret1);
    tock_exit(ret1);
  }

  printf("[Success] App flashed successfully. Creating process now.\n");
  int ret2 = libtock_app_loader_load();
  if (ret2 != RETURNCODE_SUCCESS) {
    printf("[Error] Process creation failed: %d.\n", ret2);
    tock_exit(ret2);
  }

  // wait on load done callback
  yield_for(&load_done);
  load_done = false;

  printf("[Log] Waiting for a button press.\n");
}


/******************************************************************************************************
* Function to write the app into the flash
*
* Takes app size and the app binary as arguments
******************************************************************************************************/

int write_app(double size, const uint8_t binary[]) {

  uint32_t write_count = 0;
  uint8_t write_buffer[FLASH_BUFFER_SIZE];
  uint32_t flash_offset = 0;

  // This value can be changed to different sizes
  // to mimic different bus widths.
  uint32_t write_buffer_size = FLASH_BUFFER_SIZE;

  write_count = ceil(size / write_buffer_size);

  // set the write buffer
  int ret = libtock_app_loader_set_buffer(write_buffer, FLASH_BUFFER_SIZE);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Error] Failed to set the write buffer: %d.\n", ret);
    return -1;
  }

  for (uint32_t offset = 0; offset < write_count; offset++) {
    // copy binary to write buffer
    memcpy(write_buffer, &binary[write_buffer_size * offset], write_buffer_size);
    flash_offset = (offset * write_buffer_size);
    int ret1 = libtock_app_loader_write(flash_offset, write_buffer_size);
    if (ret1 != 0) {
      printf("[Error] Failed writing data to flash at address: 0x%lx\n", flash_offset);
      printf("[Error] Error nature: %d\n", ret1);
      return -1;
    }
    // wait on write done callback
    yield_for(&write_done);
    write_done = false;
  }

  // Now that we are done writing the binary, we ask the kernel to finalize it.
  printf("[Success] Done writing app. Finalizing app image.\n");
  int ret2 = libtock_app_loader_finalize();
  if (ret2 != 0) {
    printf("[Error] Failed to finalize new process binary.\n");
    return -1;
  }
  yield_for(&finalize_done);
  finalize_done = false;

  return 0;
}


/******************************************************************************************************
*
* Main
*
******************************************************************************************************/

int main(void) {
  printf("[Log] Simple test app to load an app dynamically.\n");

  // check if app loader driver exists
  if (!libtock_app_loader_exists()) {
    printf("No App Loader driver!\n");
    return -1;
  }

  int count;
  int err = libtock_button_count(&count);
  // Ensure there is a button to use.
  if (err < 0) return err;
  printf("[Log] There are %d buttons on this board.\n", count);

  // Enable interrupts on each button.
  for (int i = 0; i < count; i++) {
    libtock_button_notify_on_press(i, button_callback);
  }

  // set up the setup done callback
  int err1 = libtock_app_loader_setup_set_upcall(app_setup_done_callback, NULL);
  if (err1 != 0) {
    printf("[Error] Failed to set setup done callback: %d\n", err1);
    return err1;
  }

  // set up the write done callback
  int err2 = libtock_app_loader_write_set_upcall(app_write_done_callback, NULL);
  if (err2 != 0) {
    printf("[Error] Failed to set flash write done callback: %d\n", err2);
    return err2;
  }

  // set up the finalize done callback
  int err3 = libtock_app_loader_finalize_set_upcall(app_finalize_done_callback, NULL);
  if (err3 != 0) {
    printf("[Error] Failed to set finalize done callback: %d\n", err3);
    return err3;
  }

  // set up the load done callback
  int err4 = libtock_app_loader_load_set_upcall(app_load_done_callback, NULL);
  if (err4 != 0) {
    printf("[Error] Failed to set load done callback: %d\n", err4);
    return err4;
  }

  printf("[Log] Waiting for a button press.\n");

  while (1) {
    if (app_load) {
      printf("[Event] Button for %s pressed!\n", app_name);
      printf("size: %d\n", app_size);
      appload(app_size, app_data);
      app_load = false;
    }
    yield();
  }
}
