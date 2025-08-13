#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/kernel/app_loader.h>
#include <libtock/kernel/ipc.h>

#include "loadable_binaries.h"

static bool setup_done    = false;    // to check if setup is done
static bool finalize_done = false;    // to check if the kernel is done finalizing the process binary
static bool load_done     = false;    // to check if the process was loaded successfully

uint8_t app_id = 0;


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

int install_binary(uint8_t id) {
  if (BINARY_COUNT == 0) {
    printf("[App Loader] No included apps. Unable to install!\n");
    return -1;
  }

  const char* app_name       = NULL;
  unsigned char* app_data    = NULL;
  size_t app_size            = 0;
  size_t binary_size         = 0;
  uint32_t write_buffer_size = 4096;

  app_name    = binary_names[id];
  app_data    = (uint8_t*)(uintptr_t)binaries[id];
  app_size    = binary_sizes[id];
  binary_size = actual_sizes[id];

  printf("[AppLoader] Requested to load %s!\n", app_name);

  int ret = libtock_app_loader_setup(app_size, app_setup_done_callback);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Error] Setup Failed: %d.\n", ret);
    tock_exit(ret);
  }

  // wait on setup done callback
  yield_for(&setup_done);
  setup_done = false;

  printf("[Success] Setup successful. Writing app to flash.\n");

  size_t offset = 0;
  while (offset < binary_size) {
    size_t chunk_len = (binary_size - offset > write_buffer_size)
                        ? write_buffer_size
                        : binary_size - offset;

    int ret1 = libtock_app_loader_write(offset, &app_data[offset], chunk_len);
    if (ret1 != RETURNCODE_SUCCESS) {
      printf("[Error] Chunk write failed at offset %zu\n", offset);
      break;
    }
    offset += chunk_len;
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

  return 0;
}


static void ipc_callback(int pid, int len, int buf, __attribute__ ((unused)) void* ud) {
  uint8_t* buffer         = (uint8_t*) (uintptr_t) buf;
  const char* name_buffer = (const char*) (uintptr_t) buf;

  int offset       = 0;
  int num_binaries = sizeof(binary_sizes) / sizeof(binary_sizes[0]);

  if (len < 1) {
    // Need at least one byte for the command.
    return;
  }

  uint8_t command = buffer[0];

  switch (command) {
    case 0:
      // Return the number of binaries available
      buffer[0] = num_binaries;
      ipc_notify_client(pid);
      break;

    case 1:
      // Return the list of binaries to display on the menu
      if (len < num_binaries + 1) {
        printf("[AppLoader] Returning on Command 0x01\n");
        return;
      }

      for (int i = 0; i < num_binaries; i++) {
        size_t name_len = strlen(binary_names[i]);

        if ((size_t)(offset + name_len + 1) > (size_t)len) {
          printf("[AppLoader] Buffer overflow risk.\n");
          return;
        }

        // Copy the binary name to the buffer
        memcpy((void*) &name_buffer[offset], binary_names[i], name_len + 1);
        offset += name_len + 1;
      }

      ipc_notify_client(pid);
      break;

    case 2:
      // install certain app
      if (len < 2) {
        // app id missing
        printf("[AppLoader] Returning on Command 0x02\n");
        return;
      }

      app_id = buffer[1];
      int ret = install_binary(app_id);
      buffer[0] = ret;
      ipc_notify_client(pid);
      break;
  }
}


int main(void) {

  if (!libtock_app_loader_exists()) {
    printf("[Error] Failed to detect App Loader Driver!\n");
    return -1;
  }

  ipc_register_service_callback("app_loader", ipc_callback,
                                NULL);

  while (1) {
    yield();
  }
}
