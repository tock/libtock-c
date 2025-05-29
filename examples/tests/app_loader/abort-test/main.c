// \file

// This is a helper program to test the dynamic app loading functionality
// of Tock works. This app has three other application binaries
// (a c-hello variant, blink and ADC)
// pre-programmed into it. When the app receives a command via console,
// the dynamic process loader enables the new app to be written to flash and
// loaded as a new process, or abort it depending on the command received.

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <libtock-sync/interface/console.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/kernel/app_loader.h>
#include <libtock/tock.h>


#define FLASH_BUFFER_SIZE   512
#define RETURNCODE_SUCCESS  0
#define CONSOLE_BUFFER_SIZE 1

uint8_t console_buffer[CONSOLE_BUFFER_SIZE];

static bool setup_done    = false;      // to check if setup is done
static bool write_done    = false;      // to check if writing to flash is done
static bool finalize_done = false;      // to check if the kernel is done finalizing the process binary
static bool load_done     = false;      // to check if the process was loaded successfully
static bool abort_done    = false;      // to check if the process binary writing
                                        // was aborted successfully

static bool abort_tracker = false;      // track when an abort was successful to stop writing
                                        // process binary data

/******************************************************************************************************
* Loadable Applications
******************************************************************************************************/

#ifdef __has_embed
static const uint8_t APP_ADC[] = {
#embed "adc.embed"
};
#else
#include "adc.xxd"
#define APP_ADC adc_embed
#endif

/******************************************************************************************************
* Function Prototypes
******************************************************************************************************/
void abort_test(void);

/******************************************************************************************************
 * Callback functions
 *
 * 1. Callback to let us know when the capsule is done writing data to flash
 * 2. Set callback to initiate the dynamic app load process on receiving command from console
 *
 *******************************************************************************************************/

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

static void app_abort_done_callback(__attribute__((unused)) int   arg0,
                                    __attribute__((unused)) int   arg1,
                                    __attribute__((unused)) int   arg2,
                                    __attribute__((unused)) void* ud) {
  abort_done = true;
}

// Callback for console commands.
void abort_test(void) {

  const uint8_t* app_data = APP_ADC;
  uint32_t app_size = sizeof(APP_ADC);
  abort_tracker = true;

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

  abort_tracker = false;
}


/******************************************************************************************************
*
* Function to write the app into the flash
*
* Takes app size and the app binary as arguments
******************************************************************************************************/

int write_app(double size, const uint8_t binary[]) {

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

  if (abort_tracker) {
    write_count = write_count / 2;
  }

  for (uint32_t offset = 0; offset < write_count; offset++) {
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

  if (!abort_tracker) {
    // Now that we are done writing the binary, we ask the kernel to finalize it.
    printf("Done writing app, finalizing.\n");
    int ret2 = libtock_app_loader_finalize();
    if (ret2 != 0) {
      printf("[Error] Failed to finalize new process binary.\n");
      return -1;
    }
    yield_for(&finalize_done);
    finalize_done = false;
  } else {
    printf("[Log] Aborting Setup/Write.\n");
    int ret0 = libtock_app_loader_abort();
    if (ret0 != RETURNCODE_SUCCESS) {
      printf("[Error] Abort Failed: %d.\n", ret0);
    }

    // wait on abort done callback
    yield_for(&abort_done);
    abort_done = false;
    printf("[Success] Abort Successful.\n");
  }
  return 0;
}


/******************************************************************************************************
*
* Main
*
******************************************************************************************************/

int main(void) {
  printf("[Log] Simple test app to test abort functionality of dynamic process loading.\n");

  // check if app loader driver exists
  if (!libtock_app_loader_exists()) {
    printf("No App Loader driver!\n");
    return -1;
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

  // set up the abort done callback
  int err5 = libtock_app_loader_abort_set_upcall(app_abort_done_callback, NULL);
  if (err5 != 0) {
    printf("[Error] Failed to set abort done callback: %d\n", err5);
    return err5;
  }

  libtocksync_alarm_delay_ms(5000);

  printf("[Log] Initiating Abort Test.\n");

  abort_test();

  printf("[Log] Exiting Abort Test.\n");
  return 0;
}
