// \file

// This is a helper program to test if the in-place process loading functionality
// of Tock works. When the user presses a button on a supported device, the in-place
// process loader loads as a new process using the binary at address specified by the
// userland application.

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/interface/button.h>
#include <libtock/internal/in_place_process_loader.h>
#include <libtock/tock.h>


#define RETURNCODE_SUCCESS 0

static bool load_done = false;      // to check if the process was loaded successfully

/******************************************************************************************************
* Callback functions
* Set button callback to initiate the dynamic app load process on pressing button 1 (on nrf52840dk)
*
******************************************************************************************************/

// static void nop_callback(int a __attribute__((unused)), int b __attribute__((unused)), int c __attribute__((unused)), void *d __attribute__((unused))) {}

static void in_place_process_load_done_callback(__attribute__((unused)) int   length,
                                                __attribute__((unused)) int   arg1,
                                                __attribute__((unused)) int   arg2,
                                                __attribute__((unused)) void* ud) {
  load_done = true;
}

static void button_callback(__attribute__ ((unused)) returncode_t retval, int btn_num, bool pressed) {
  // Callback for button presses.
  // val: 1 if pressed, 0 if depressed

  // flash and load blink
  if (btn_num == BUTTON1 && pressed == 1) {

    libtocksync_alarm_delay_ms(100);  // debounce

    if (pressed == 1) {

      printf("[Event] Button 1 Pressed!\n");
      int ret = 0;
      uint32_t app_address = 344064;
      uint32_t app_size    = 2048;

      printf("[Log] Attempting to create a process now.\n");
      ret = in_place_process_loader_command_load(app_address, app_size);   // request to load app
      if (ret != RETURNCODE_SUCCESS) {
        printf("[Error] Process creation failed: %d.\n", ret);
        printf("[Log] Exiting Application.\n");
        tock_exit(ret);   // we failed, so we exit the program.
      } else {
        printf("[Success] Process created successfully.\n");
      }
      app_address = 0;// reset app_address
      app_size    = 0; // reset app_size
    }
  }
}


/******************************************************************************************************
*
* Main
*
******************************************************************************************************/

int main(void) {
  printf("[Log] Simple test app to create a process out of a binary present in flash during runtime.\n");

  int count;
  int err = libtock_button_count(&count);
  // Ensure there is a button to use.
  if (err < 0) return err;
  printf("[Log] There are %d buttons on this board.\n", count);

  // Enable interrupts on each button.
  for (int i = 0; i < count; i++) {
    libtock_button_notify_on_press(i, button_callback);
  }

  // set up the load done callback
  int err1 = in_place_process_loader_load_subscribe(in_place_process_load_done_callback, NULL);
  if (err1 != 0) {
    printf("[Error] Failed to set load done callback: %d\n", err1);
    return err1;
  }

  // Check if the app_loader driver exists.
  int ret;
  ret = in_place_process_loader_exists();
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Error] In Place Process Loader driver does not exist.\n");
    return ret; // the driver does not exist, so we cannot load an app anyway. Let us exit the program.
  }

  printf("[Log] Waiting for a button press.\n");

  while (1) {
    yield();
  }
}
