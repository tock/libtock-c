// \file

// This is a helper program to test the dynamic app loading functionality
// of Tock works. This app has two other applications' (blink and ADC) binaries
// pre-programmed into it. When the user presses a button on a supported device,
// the dynamic process loader enables the new app to be written to flash and
// loaded as a new process.

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <examples/tests/app_loader/button-press-loading/pre-compiled-binaries/app_binaries.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/interface/button.h>
#include <libtock/kernel/app_loader.h>
#include <libtock/tock.h>


#define FLASH_BUFFER_SIZE 512
#define RETURNCODE_SUCCESS 0

static bool setup_done = false;     // to check if setup is done
static bool write_done = false;     // to check if writing to flash is done
static bool load_done  = false;     // to check if the process was loaded successfully
static bool abort_done = false;     // to check if the process binary writing
                                    // was aborted successfully

static bool abort_tracker = false;  // track when an abort was successful to stop writing
                                    // process binary data

/******************************************************************************************************
* Callback functions
*
* 1. Callback to let us know when the capsule is done writing data to flash
* 2. Set button callback to initiate the dynamic app load process on pressing button 1 (on nrf52840dk)
*
******************************************************************************************************/

// static void nop_callback(int a __attribute__((unused)), int b __attribute__((unused)), int c __attribute__((unused)), void *d __attribute__((unused))) {}

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

static void app_load_done_callback(__attribute__((unused)) int   arg0,
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

static void app_abort_done_callback(__attribute__((unused)) int   arg0,
                                    __attribute__((unused)) int   arg1,
                                    __attribute__((unused)) int   arg2,
                                    __attribute__((unused)) void* ud) {
  abort_done = true;
}

// Callback for button presses.
static void button_callback(__attribute__ ((unused)) returncode_t retval, int btn_num, bool pressed) {
  // Callback for button presses.
  // val: 1 if pressed, 0 if depressed

  if (pressed == 1) {
    // debounce
    // Note: A value of 100ms is chosen
    // because the current alarm upcall implementation
    // results in panic when a button is pressed within
    // the debounce period if the debounce period is long.
    // Setting the debounce interval to 100ms seems to work
    // but setting it to 200ms and rapidly clicking buttons
    // leads to the kernel panicking.
    libtocksync_alarm_delay_ms(100);

    if (pressed == 1) {
      const char* app_name    = NULL;
      unsigned char* app_data = NULL;
      double app_size         = 0;
      int ret0 = 0;

      switch (btn_num) {
        case BUTTON1:
          app_name = "Blink";
          app_data = blink;
          app_size = sizeof(blink);
          break;
        case BUTTON2:
          app_name = "ADC";
          app_data = adc;
          app_size = sizeof(adc);
          break;
        case BUTTON3:
          printf("[Log] Aborting Setup/Write.\n");
          ret0 = libtock_app_loader_abort();
          if (ret0 != RETURNCODE_SUCCESS) {
            printf("[Error] Abort Failed: %d.\n", ret0);
          }
          // wait on abort done callback
          yield_for(&abort_done);
          abort_done    = false;
          abort_tracker = true;
          printf("[Success] Setup/Write aborted successfully.\n");
          return;
        default:
          return;
      }
      printf("[Event] Button for %s Pressed!\n", app_name);
      abort_tracker = false;

      int ret = libtock_app_loader_setup(app_size);
      if (ret != RETURNCODE_SUCCESS) {
        printf("[Error] Setup Failed: %d.\n", ret);
        tock_exit(ret);
      }

      // wait on setup done callback
      yield_for(&setup_done);
      setup_done = false;

      printf("[Success] Setup successful. Writing app to flash.\n");
      int ret1 = write_app(app_size, app_data);
      if (ret1 != RETURNCODE_SUCCESS) {
        printf("[Error] App flash write unsuccessful: %d.\n", ret1);
        tock_exit(ret1);
      }

      if (!abort_tracker) {
        printf("[Success] App flashed successfully. Creating process now.\n");
        int ret2 = libtock_app_loader_load();
        if (ret2 != RETURNCODE_SUCCESS) {
          printf("[Error] Process creation failed: %d.\n", ret2);
          tock_exit(ret2);
        }

        // wait on load done callback
        yield_for(&load_done);
        load_done = false;
      }
      printf("[Log] Waiting for a button press.\n");
    }
  }
}


/******************************************************************************************************
*
* Function to write the app into the flash
*
* Takes app size and the app binary as arguments
******************************************************************************************************/

int write_app(double size, uint8_t binary[]) {

  int ret;
  uint32_t write_count = 0;
  uint8_t write_buffer[FLASH_BUFFER_SIZE];
  uint32_t flash_offset = 0;

  write_count = ceil(size / FLASH_BUFFER_SIZE);

  // set the write buffer
  ret = libtock_app_loader_set_buffer(write_buffer, FLASH_BUFFER_SIZE);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Error] Failed to set the write buffer: %d.\n", ret);
    return -1;
  }

  for (uint32_t offset = 0; offset < write_count; offset++) {
    if (abort_tracker) {
      break;
    }
    // copy binary to write buffer
    memcpy(write_buffer, &binary[FLASH_BUFFER_SIZE * offset], FLASH_BUFFER_SIZE);
    flash_offset = (offset * FLASH_BUFFER_SIZE);
    ret = libtock_app_loader_write(flash_offset, FLASH_BUFFER_SIZE);
    if (ret != 0) {
      printf("[Error] Failed writing data to flash at address: 0x%lx\n", flash_offset);
      printf("[Error] Error nature: %d\n", ret);
      return -1;
    }
    // wait on write done callback
    yield_for(&write_done);
    write_done = false;
  }
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
  int err1 = libtock_app_loader_set_setup_upcall(app_setup_done_callback, NULL);
  if (err1 != 0) {
    printf("[Error] Failed to set setup done callback: %d\n", err1);
    return err1;
  }

  // set up the write done callback
  int err2 = libtock_app_loader_set_write_upcall(app_write_done_callback, NULL);
  if (err2 != 0) {
    printf("[Error] Failed to set flash write done callback: %d\n", err2);
    return err2;
  }

  // set up the load done callback
  int err3 = libtock_app_loader_set_load_upcall(app_load_done_callback, NULL);
  if (err3 != 0) {
    printf("[Error] Failed to set load done callback: %d\n", err3);
    return err3;
  }

  // set up the abort done callback
  int err4 = libtock_app_loader_set_abort_upcall(app_abort_done_callback, NULL);
  if (err4 != 0) {
    printf("[Error] Failed to set abort done callback: %d\n", err4);
    return err4;
  }

  printf("[Log] Waiting for a button press.\n");

  while (1) {
    yield();
  }
}
