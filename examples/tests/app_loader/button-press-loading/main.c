#include <math.h>
#include <stdio.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/interface/button.h>
#include <libtock/kernel/app_loader.h>
#include <libtock/tock.h>

#include "loadable_binaries.h"

/******************************************************************************************************
* Callback Tracking Flags
******************************************************************************************************/
static bool setup_done    = false;   // to check if setup is done
static bool finalize_done = false;   // to check if the process was finalized successfully
static bool load_done     = false;   // to check if the process was loaded successfully
static bool app_load      = false;   // to check if there is a request to load a new app

/******************************************************************************************************
* Variables to hold loadable app information
******************************************************************************************************/
const char* new_app_name    = NULL;
unsigned char* new_app_data = NULL;
size_t new_app_size         = 0;
size_t new_binary_size      = 0;


/******************************************************************************************************
* Callback functions
*
* Set button callback to initiate the dynamic app load process on pressing buttons
******************************************************************************************************/

static void app_setup_done_callback(__attribute__((unused)) int   arg0,
                                    __attribute__((unused)) int   arg1,
                                    __attribute__((unused)) int   arg2,
                                    __attribute__((unused)) void* ud) {
  setup_done = true;
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
static void button_callback(__attribute__ ((unused)) returncode_t retval, int btn_num, __attribute__ (
                              (unused)) bool pressed) {
  // Callback for button presses.
  if (btn_num < BINARY_COUNT) {
    app_load        = true;
    new_app_name    = binary_names[btn_num];
    new_app_data    = (uint8_t*)(uintptr_t)binaries[btn_num];
    new_app_size    = binary_sizes[btn_num];
    new_binary_size = actual_sizes[btn_num];
  } else {
    printf("[App Loader] Invalid button selected. Unable to install!\n");
  }
}


/******************************************************************************************************
* Helper Function for the apploader machine
*
* Takes app size and the app binary as arguments
******************************************************************************************************/

static void appload(double binary_size, double app_size, uint8_t binary[]) {
  int ret = libtock_app_loader_setup(app_size, app_setup_done_callback);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Error] Setup Failed: %d.\n", ret);
    tock_exit(ret);
  }

  // wait on setup done callback
  yield_for(&setup_done);
  setup_done = false;

  printf("[Success] Setup successful. Writing app to flash.\n");
  int ret1 = libtock_app_loader_write(binary_size, binary);
  if (ret1 != RETURNCODE_SUCCESS) {
    printf("[Error] App flash write unsuccessful: %d.\n", ret1);
    tock_exit(ret1);
  }

  int ret2 = libtock_app_loader_finalize(app_finalize_done_callback);
  if (ret2 != RETURNCODE_SUCCESS) {
    printf("[Error] Finalizing app failed: %d.\n", ret2);
    tock_exit(ret2);
  }

  // wait on finalize done callback
  yield_for(&finalize_done);
  finalize_done = false;

  printf("[Success] App flashed successfully. Creating process now.\n");
  int ret3 = libtock_app_loader_load(app_load_done_callback);
  if (ret3 != RETURNCODE_SUCCESS) {
    printf("[Error] Process creation failed: %d.\n", ret3);
    tock_exit(ret3);
  }

  // wait on load done callback
  yield_for(&load_done);
  load_done = false;

  printf("[Log] Waiting for a button press.\n");
}

/******************************************************************************************************
* Main
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

  printf("[Log] Waiting for a button press.\n");

  while (1) {
    if (app_load) {
      printf("[Event] Button for %s pressed!\n", new_app_name);
      printf("size: %d bytes\n", new_app_size);
      appload(new_binary_size, new_app_size, new_app_data);
      app_load = false;
    }
    yield();
  }
}
