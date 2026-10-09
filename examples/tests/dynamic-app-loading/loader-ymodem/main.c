#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/kernel/app_loader.h>

#include "ymodem.h"

#define FLASH_BUFFER_SIZE 512
#define RETURNCODE_SUCCESS 0

static bool setup_done    = false;    // to check if setup is done
static bool write_done    = false;    // to check if writing to flash is done
static bool finalize_done = false;    // to check if the kernel is done finalizing the process binary
static bool load_done     = false;    // to check if the process was loaded successfully

uint8_t app_id = 0;


// Scratch space for one data block. Static (not stack-allocated) since
// 1024 bytes may not comfortably fit alongside other locals on a small
// app stack.
static uint8_t block_data[YM_BLOCK_SIZE_LONG];

/********************************
 * Function prototypes
 *********************************/
int install_binary(uint8_t id);


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



uint32_t ymodem_offset = 0;

static void ymodem_file_started_callback(uint32_t filesize) {
  printf("[AppLoader] ymodem started app size %i!\n", filesize);
  ymodem_offset = 0;


  int ret = libtock_app_loader_setup(filesize);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[AppLoader] Error: Setup Failed: %d.\n", ret);
    return;
  }
}



static void ymodem_block_received_callback(uint8_t* buffer, uint32_t len) {
  int ret;

  printf("[AppLoader] ymodem writing block [%i:%i]\n", ymodem_offset, ymodem_offset+len);


   ret = libtock_app_loader_set_buffer(buffer, len);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[AppLoader] Error: Failed to set the write buffer: %d.\n", ret);
    return;
  }

  write_done = false;
   ret = libtock_app_loader_write(ymodem_offset, len);
  if (ret != 0) {
    printf("[AppLoader] Error: Failed writing data to flash at address: 0x%lx\n", ymodem_offset);
    printf("[AppLoader] Error: %s (%d)\n", tock_strrcode(ret), ret);
    return;
  }
  // wait on write done callback
  yield_wait_for(0x10001,1);
  printf("[AppLoader] block written\n");

  ymodem_offset+=len;

}

static void ymodem_file_received_callback(void) {
  int ret;
  printf("[AppLoader] Ymodem file received. Creating process now.\n");

    // Now that we are done writing the binary, we ask the kernel to finalize it.
  printf("Done writing app, finalizing.\n");
   finalize_done = false;
   ret = libtock_app_loader_finalize();
  if (ret != 0) {
    printf("[Error] Failed to finalize new process binary.\n");
    return;
  }
  yield_for(&finalize_done);
  

  load_done = false;
   ret = libtock_app_loader_load();
  if (ret != RETURNCODE_SUCCESS) {
    printf("[AppLoader] Error: Process creation failed: %d.\n", ret);
    return;
  }

  // wait on load done callback
  yield_for(&load_done);
  
}


int write_app(double size, uint8_t binary[]) {

  uint32_t write_count = 0;
  uint8_t write_buffer[FLASH_BUFFER_SIZE];
  uint32_t flash_offset = 0;

  // This value can be changed to different sizes
  // to mimic different bus widths.
  uint32_t write_buffer_size = FLASH_BUFFER_SIZE;

  // set the write buffer
  int ret = libtock_app_loader_set_buffer(write_buffer, FLASH_BUFFER_SIZE);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Error] Failed to set the write buffer: %d.\n", ret);
    return -1;
  }

  write_count = (size + write_buffer_size - 1) / write_buffer_size;

  for (uint32_t offset = 0; offset < write_count; offset++) {

    memset(write_buffer, 0, write_buffer_size);
    // copy binary to write buffer
    flash_offset = (offset * write_buffer_size);
    size_t bytes_left = size - flash_offset;
    size_t chunk      = bytes_left < write_buffer_size ? bytes_left : write_buffer_size;
    memcpy(write_buffer, &binary[write_buffer_size * offset], chunk);
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
  printf("Done writing app, finalizing.\n");
  int ret2 = libtock_app_loader_finalize();
  if (ret2 != 0) {
    printf("[Error] Failed to finalize new process binary.\n");
    return -1;
  }
  yield_for(&finalize_done);
  finalize_done = false;

  return 0;
}



int main(void) {

  if (!libtock_app_loader_exists()) {
    printf("[Error] Failed to detect App Loader Driver!\n");
    return -1;
  }

  // set up the setup done callback
  int err1 = libtock_app_loader_subscribe_setup(app_setup_done_callback, NULL);
  if (err1 != 0) {
    printf("[Error] Failed to set setup done callback: %d\n", err1);
    return err1;
  }

  // set up the write done callback
  int err2 = libtock_app_loader_subscribe_write(app_write_done_callback, NULL);
  if (err2 != 0) {
    printf("[Error] Failed to set flash write done callback: %d\n", err2);
    return err2;
  }

  // set up the finalize done callback
  int err3 = libtock_app_loader_subscribe_finalize(app_finalize_done_callback, NULL);
  if (err3 != 0) {
    printf("[Error] Failed to set finalize done callback: %d\n", err3);
    return err3;
  }

  // set up the load done callback
  int err4 = libtock_app_loader_subscribe_load(app_load_done_callback, NULL);
  if (err4 != 0) {
    printf("[Error] Failed to set load done callback: %d\n", err4);
    return err4;
  }

  ymodem_start(block_data, ymodem_file_started_callback, ymodem_block_received_callback, ymodem_file_received_callback);

  while (1) {
    yield();
  }
}
