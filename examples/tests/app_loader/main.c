// \file

// This is a helper program to test if the dynamic app loading functionality
// of Tock works. This app has another application's (blink) binary pre-programmed
// into it. When the user presses a button on a supported device, the dynamic
// process loader enables the new app to be written to flash and loaded as a new process.

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <app_binaries.h>
#include <app_loader.h>
#include <button.h>
#include <timer.h>
#include <tock.h>


#define FLASH_BUFFER_SIZE 512
#define RETURNCODE_SUCCESS 0

static bool setup_done = false;     // to check if setup is done
static bool write_done = false;     // to check if writing to flash is done
static bool load_done  = false;     // to check if the process was loaded successfully

/******************************************************************************************************
* Callback functions
*
* 1. Callback to let us know when the capsule is done writing data to flash
* 2. Set button callback to initiate the dynamic app load process on pressing button 1 (on nrf52840dk)
*
******************************************************************************************************/

// static void nop_callback(int a __attribute__((unused)), int b __attribute__((unused)), int c __attribute__((unused)), void *d __attribute__((unused))) {}

static void app_setup_done_callback(__attribute__((unused)) int   length,
                                    __attribute__((unused)) int   arg1,
                                    __attribute__((unused)) int   arg2,
                                    __attribute__((unused)) void *ud)
{
  setup_done = true;
}

static void app_write_done_callback(__attribute__((unused)) int   length,
                                    __attribute__((unused)) int   arg1,
                                    __attribute__((unused)) int   arg2,
                                    __attribute__((unused)) void *ud)
{
  write_done = true;
}

static void app_load_done_callback( __attribute__((unused)) int   length,
                                    __attribute__((unused)) int   arg1,
                                    __attribute__((unused)) int   arg2,
                                    __attribute__((unused)) void *ud)
{
  load_done = true;
}

static void button_callback(int                            btn_num,
                            int                            val,
                            __attribute__ ((unused)) int   arg2,
                            __attribute__ ((unused)) void *ud)
{
  // Callback for button presses.
  // val: 1 if pressed, 0 if depressed

  // flash and load blink
  if (btn_num == BUTTON1 && val == 1) {

    delay_ms(100);  // debounce

    if (val == 1) {

      printf("[Event] Button 1 Pressed!\n");
      double app_size = sizeof(blink);
      int ret         = 0;
      int ret1        = 0;
      int ret2        = 0;

      ret = app_loader_command_setup(app_size); // asks the capsule to set up for app flash
      if (ret != RETURNCODE_SUCCESS) {
        printf("[Error] Setup Failed: %d.\n", ret);
        printf("[Log] Exiting Application.\n");
        tock_exit(ret); // we failed, so we exit the program.
      } else {
        printf("[Log] Yielding for setup done.\n");
        yield_for(&setup_done);   // wait until the padding app write is done before you send your app, or it will fail during write
        setup_done = false;
        printf("[Success] Setup successful. Attempting to write app to flash now.\n");
        ret1 = write_app(app_size, blink); // writes app data to flash
        if (ret1 != RETURNCODE_SUCCESS) {
          printf("[Error] App flash write unsuccessful: %d.\n", ret1);
          printf("[Log] Exiting Application.\n");
          tock_exit(ret1); // we failed, so we exit the program.
        } else {
          printf("[Success] App flashed successfully. Attempting to create a process now.\n");
          ret2 = app_loader_command_load();   // request to load app
          if (ret2 != RETURNCODE_SUCCESS) {
            printf("[Error] Process creation failed: %d.\n", ret2);
            printf("[Log] Exiting Application.\n");
            tock_exit(ret2);   // we failed, so we exit the program.
          } else {
            printf("[Success] Process created successfully.\n");
          }
        }
      }
      app_size = 0;  // reset app_size
    }
  }
  // flash and load adc
  else if (btn_num == BUTTON2 && val == 1) {

    delay_ms(100);  // debounce

    if (val == 1) {
      printf("[Event] Button 2 Pressed!\n");
      double app_size = sizeof(adc);
      int ret         = 0;
      int ret1        = 0;
      int ret2        = 0;

      ret = app_loader_command_setup(app_size); // asks the capsule to set up for app flash
      if (ret != RETURNCODE_SUCCESS) {
        printf("[Error] Setup Failed: %d.\n", ret);
        printf("[Log] Exiting Application.\n");
        tock_exit(ret); // we failed, so we exit the program.
      } else {
        // yielding for padding write
        printf("[Log] yielding for setup done.\n");
        yield_for(&setup_done);   // wait until the padding app write is done before you send your app, or it will fail during write
        setup_done = false;
        // padding success
        printf("[Success] Setup successful. Attempting to write app to flash now.\n");
        ret1 = write_app(app_size, adc); // writes app data to flash
        if (ret1 != RETURNCODE_SUCCESS) {
          printf("[Error] App flash write unsuccessful: %d.\n", ret1);
          printf("[Log] Exiting Application.\n");
          tock_exit(ret1); // we failed, so we exit the program.
        } else {
          printf("[Success] App flashed successfully. Attempting to create a process now.\n");
          ret2 = app_loader_command_load();   // request to load app
          if (ret2 != RETURNCODE_SUCCESS) {
            printf("[Error] Process creation failed: %d.\n", ret2);
            printf("[Log] Exiting Application.\n");
            tock_exit(ret2);   // we failed, so we exit the program.
          } else {
            printf("[Success] Process created successfully.\n");
          }
        }
      }
      app_size = 0;  // reset app_size
    }
  }
}


/******************************************************************************************************
*
* Function to write the app into the flash
*
* Takes app size and the app binary as arguments
******************************************************************************************************/

int write_app(double size, uint8_t binary[]){

  int ret;
  uint32_t write_count = 0;
  uint8_t write_buffer[FLASH_BUFFER_SIZE];
  uint32_t flash_offset = 0;

  write_count = ceil(size / FLASH_BUFFER_SIZE);

  ret = app_loader_write_buffer(write_buffer, FLASH_BUFFER_SIZE);   // set the write buffer
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Error] Failed to set the write buffer: %d.\n", ret);
    return -1;
  }

  for (uint32_t offset = 0; offset < write_count; offset++) {
    memcpy(write_buffer, &binary[FLASH_BUFFER_SIZE * offset], FLASH_BUFFER_SIZE);     // copy binary to write buffer
    flash_offset = (offset * FLASH_BUFFER_SIZE);
    ret = app_loader_command_write(flash_offset, FLASH_BUFFER_SIZE);
    if (ret != 0) {
      printf("[Error] Failed writing data to flash at address: 0x%lx\n", flash_offset);
      printf("[Error] Error nature: %d\n", ret);
      return -1;
    }
    yield_for(&write_done);     // wait until write done callback
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

  int count;
  int err = button_count(&count);
  // Ensure there is a button to use.
  if (err < 0) return err;
  printf("[Log] There are %d buttons on this board.\n", count);

  // Enable interrupts on each button.
  for (int i = 0; i < count; i++) {
    button_enable_interrupt(i);
  }

  // set up the setup done callback
  int err1 = app_loader_setup_subscribe(app_setup_done_callback, NULL);
  if (err1 != 0) {
    printf("[Error] Failed to set setup done callback: %d\n", err1);
    return err1;
  }

  // set up the write done callback
  int err2 = app_loader_write_subscribe(app_write_done_callback, NULL);
  if (err2 != 0) {
    printf("[Error] Failed to set flash write done callback: %d\n", err2);
    return err2;
  }

  // set up the load done callback
  int err3 = app_loader_load_subscribe(app_load_done_callback, NULL);
  if (err3 != 0) {
    printf("[Error] Failed to set load done callback: %d\n", err3);
    return err3;
  }

  // setup the button press callback
  int err4 = button_subscribe(button_callback, NULL);
  if (err4 != 0) {
    printf("[Error] Failed to set button callback: %d.\n", err4);
    return err4;
  }

  // Check if the app_loader driver exists.
  int ret;
  ret = app_loader_exists();
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Error] Dynamic Apploader driver does not exist.\n");
    return ret; // the driver does not exist, so we cannot load an app anyway. Let us exit the program.
  }

  printf("[Log] Waiting for a button press.\n");

  while (1) {
    yield();
  }
}
