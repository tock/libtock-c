#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/kernel/app_loader.h>
#include <libtock-sync/services/alarm.h>

#include "ymodem.h"

uint8_t app_id = 0;


// Scratch space for one data block. Static (not stack-allocated) since
// 1024 bytes may not comfortably fit alongside other locals on a small
// app stack.
static uint8_t block_data[YM_BLOCK_SIZE_LONG];


uint32_t ymodem_offset = 0;

static void ymodem_file_started_callback(uint32_t filesize) {
  int ret;
  printf("[AppLoader] ymodem started app size %" PRIu32 "!\n", filesize);

  ymodem_offset = 0;

  ret = libtocksync_app_loader_setup(filesize);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[AppLoader] Error: Setup Failed: %d.\n", ret);
    return;
  }
}

static void ymodem_block_received_callback(uint8_t* buffer, uint32_t len) {
  int ret;
  printf("[AppLoader] ymodem writing block [%" PRIu32 ":%" PRIu32 "]\n", ymodem_offset, ymodem_offset + len);

  ret = libtocksync_app_loader_write(ymodem_offset, len, buffer, len);
  if (ret != 0) {
    printf("[AppLoader] Error: Failed writing data to flash at address: 0x%" PRIx32 "\n", ymodem_offset);
    printf("[AppLoader] Error: %s (%d)\n", tock_strrcode(ret), ret);
    return;
  }

  ymodem_offset += len;
}

static void ymodem_file_received_callback(void) {
  int ret;
  printf("[AppLoader] ymodem file received. Creating process now.\n");

  // Now that we are done writing the binary, we ask the kernel to finalize it.
  printf("[AppLoader] Done writing app, finalizing.\n");
  ret = libtocksync_app_loader_finalize();
  if (ret != 0) {
    printf("[Error] Failed to finalize new process binary.\n");
    return;
  }

  printf("[AppLoader] Done finalizing, loading.\n");
  ret = libtocksync_app_loader_load();
  if (ret != RETURNCODE_SUCCESS) {
    printf("[AppLoader] Error: Process creation failed: %d.\n", ret);
    return;
  }
}

int main(void) {
  if (!libtocksync_app_loader_exists()) {
    printf("[Error] Failed to detect App Loader Driver!\n");
    return -1;
  }

  while (1) {
    ymodem_start(block_data, ymodem_file_started_callback, ymodem_block_received_callback,
                 ymodem_file_received_callback);
  }
}
