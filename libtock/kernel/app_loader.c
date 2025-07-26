#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <libtock/tock.h>

#include "app_loader.h"

#define FLASH_BUFFER_SIZE 4096

static bool write_done   = false;
volatile bool abort_flag = false;
static bool write_upcall_registered = false;

/******************************************************************************************************
 * Callback functions
 *
 * Set callback to track write done signal from the kernel
 *******************************************************************************************************/

static void write_done_callback(__attribute__((unused)) int   arg0,
                                __attribute__((unused)) int   arg1,
                                __attribute__((unused)) int   arg2,
                                __attribute__((unused)) void* ud) {
  write_done = true;
}

/******************************************************************************************************
* Function to setup for the new app install
*
* Takes app size and the callback function as arguments
******************************************************************************************************/

returncode_t libtock_app_loader_setup(uint32_t app_size, subscribe_upcall cb) {
  // set up the setup done callback
  int err = libtock_app_loader_set_setup_upcall(cb, NULL);
  if (err != 0) {
    printf("[Error] Failed to set setup done callback: %d\n", err);
    return err;
  }

  return libtock_app_loader_command_setup(app_size);
}


/******************************************************************************************************
* Function to write app binary chunks into the flash
*
* Takes offset, binary chunk and the chunk length as arguments
* The user has to keep track of the offset, and has control
* over the sequence of bytes being written
******************************************************************************************************/

returncode_t libtock_app_loader_write(uint32_t offset, uint8_t* chunk_data, size_t chunk_len) {
  static uint8_t write_buffer[FLASH_BUFFER_SIZE];

  if (chunk_len > FLASH_BUFFER_SIZE) {
    return RETURNCODE_FAIL;
  }

  if (!write_upcall_registered) {
    // set up the write done callback
    libtock_app_loader_set_write_upcall(write_done_callback, NULL);
    write_upcall_registered = true;
  }

  int ret = libtock_app_loader_write_buffer(write_buffer, FLASH_BUFFER_SIZE);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Error] Failed to set the write buffer: %d.\n", ret);
    return ret;
  }

  memcpy(write_buffer, chunk_data, chunk_len);
  if (chunk_len < FLASH_BUFFER_SIZE) {
    memset(write_buffer + chunk_len, 0, FLASH_BUFFER_SIZE - chunk_len);
  }

  ret = libtock_app_loader_command_write(offset, FLASH_BUFFER_SIZE);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Error] Flash write failed at offset 0x%lx: err %d\n", offset, ret);
    return ret;
  }

  yield_for(&write_done);
  write_done = false;

  return RETURNCODE_SUCCESS;
}

/******************************************************************************************************
* Function to finalize the binary for the new app
*
* Takes callback function as argument
******************************************************************************************************/

returncode_t libtock_app_loader_finalize(subscribe_upcall cb) {
  write_upcall_registered = false;
  // set up the finalize done callbacks
  int err = libtock_app_loader_set_finalize_upcall(cb, NULL);
  if (err != 0) {
    printf("[Error] Failed to set finalize done callback: %d\n", err);
    return err;
  }

  return libtock_app_loader_command_finalize();
}

/******************************************************************************************************
* Function to create a process for the newly installed app
*
* Takes callback function as argument
******************************************************************************************************/

returncode_t libtock_app_loader_load(subscribe_upcall cb) {
  // set up the load done callback
  int err = libtock_app_loader_set_load_upcall(cb, NULL);
  if (err != 0) {
    printf("[Error] Failed to set load done callback: %d\n", err);
    return err;
  }

  return libtock_app_loader_command_load();
}

/******************************************************************************************************
* Function to abort the setup/writing of new app binary
*
* Takes callback function as argument
******************************************************************************************************/

returncode_t libtock_app_loader_abort(subscribe_upcall cb) {
  // set up the abort done callback
  abort_flag = true;

  int err = libtock_app_loader_set_abort_upcall(cb, NULL);
  if (err != 0) {
    printf("[Error] Failed to set abort done callback: %d\n", err);
    return err;
  }

  return libtock_app_loader_command_abort();
}

/******************************************************************************************************
* Function to uninstall an existing application
*
* Takes the short id and version of the app and the callback function as arguments
******************************************************************************************************/

returncode_t libtock_app_loader_uninstall(uint32_t app_short_id, uint32_t app_version, subscribe_upcall cb) {
  // set up the uninstall done callback
  int err = libtock_app_loader_set_uninstall_upcall(cb, NULL);
  if (err != 0) {
    printf("[Error] Failed to set setup done callback: %d\n", err);
    return err;
  }

  return libtock_app_loader_command_uninstall(app_short_id, app_version);
}
