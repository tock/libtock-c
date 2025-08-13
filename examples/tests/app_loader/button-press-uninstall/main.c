#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/interface/button.h>
#include <libtock/kernel/app_loader.h>
#include <libtock/tock.h>

/******************************************************************************************************
* Short ID and version of app that has to be uninstalled (Blink in this case)
******************************************************************************************************/
const uint32_t short_id = 4246331976;
const uint32_t version  = 0;

/******************************************************************************************************
* Callback Tracking Flags
******************************************************************************************************/
static bool uninstall_done = false;   // to check if setup is done
static bool app_uninstall  = false;   // to track if app has to be uninstalled

/******************************************************************************************************
* Callback functions
*
* Set button callback to initiate the uninstall on pressing buttons
******************************************************************************************************/

static void app_uninstall_done_callback(__attribute__((unused)) int   arg0,
                                        __attribute__((unused)) int   arg1,
                                        __attribute__((unused)) int   arg2,
                                        __attribute__((unused)) void* ud) {

  if (arg0 != RETURNCODE_SUCCESS) {
    printf("[Error] Uninstall failed: %d.\n", arg0);
  } else {
    printf("[Success] Uninstalled app successfully.\n");
  }
  uninstall_done = true;
}

// Callback for button presses.
static void button_callback(__attribute__ ((unused)) returncode_t retval, int btn_num, __attribute__ (
                              (unused)) bool pressed) {
  // Callback for button presses.
  if (btn_num == 0) {
    app_uninstall = true;
  } else {
    printf("[App Uninstaller] Invalid button selected. Unable to uninstall!\n");
  }
}


/******************************************************************************************************
* Main
******************************************************************************************************/

int main(void) {
  printf("[Log] Simple test app to uninstall an app during runtime.\n");

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
    if (app_uninstall) {
      printf("Uninstalling app with Short ID: %" PRIu32 " and version %" PRIu32 "\n", short_id, version);
      int ret = libtock_app_loader_uninstall(short_id, version, app_uninstall_done_callback);
      if (ret != RETURNCODE_SUCCESS) {
        printf("[Error] Uninstall Failed: %d.\n", ret);
        tock_exit(ret);
      }
      yield_for(&uninstall_done);
      uninstall_done = false;
      app_uninstall  = false;
    }
    yield();
  }
}
