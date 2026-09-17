#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/kernel/app_loader.h>
#include <libtock/kernel/ipc.h>

#include "loadable_binaries.h"

#define RETURNCODE_SUCCESS 0

// uint8_t app_id = 0;
// uint32_t short_id = 0;

typedef struct {
  uint32_t app_handle;
} Unload_metadata;

Unload_metadata um;

uint32_t write_buffer_size = 512;

/******************************************************************************************************
* Callback Tracking Flags
******************************************************************************************************/
static bool setup_done    = false;    // to check if setup is done
static bool finalize_done = false;    // to check if the process was finalized
static bool load_done     = false;    // to check if the process was loaded
// static bool app_load       = false;   // to check if there is a request to load a new app
static bool unload_done    = false;   // to check if the process was unloaded
static bool uninstall_done = false;   // to check if the application binary was uninstalled
// static bool app_uninstall  = false;   // to check if there is a request to unload and uninstall an app

/********************************
 * Function prototypes
 *********************************/
int install_binary(uint8_t id);
int uninstall_application(uint32_t id);
// int write_app(double size, uint8_t binary[]);


/******************************************************************************************************
* Callback functions
*
* Set button callback to initiate the unload on pressing buttons
******************************************************************************************************/
// Callback for setup.
static void app_setup_done_callback(__attribute__((unused)) int   arg0,
                                    __attribute__((unused)) int   arg1,
                                    __attribute__((unused)) int   arg2,
                                    __attribute__((unused)) void* ud) {
  setup_done = true;
}

// Callback for finalize.
static void app_finalize_done_callback(__attribute__((unused)) int   arg0,
                                       __attribute__((unused)) int   arg1,
                                       __attribute__((unused)) int   arg2,
                                       __attribute__((unused)) void* ud) {
  finalize_done = true;
}

// Callback for load.
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

// Callback for unload.
static void app_unload_done_callback(int                           arg0,
                                     uint32_t                      arg1,
                                     __attribute__((unused)) int   arg2,
                                     __attribute__((unused)) void* ud) {

  if (arg0 != RETURNCODE_SUCCESS) {
    printf("[Error] unload failed: %d.\n", arg0);
  } else {
    printf("[Success] unloaded app successfully. App Identifier: %" PRIu32 "\n", arg1);
  }
  um.app_handle = arg1;

  // printf("App handle ->  %" PRIu32 "\n", um.app_handle);
  unload_done = true;
}

// Callback for uninstall.
static void app_uninstall_done_callback(int                           arg0,
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


int install_binary(uint8_t id) {
  if (BINARY_COUNT == 0) {
    printf("[App Loader] No included apps. Unable to install!\n");
    return -1;
  }

  const char* app_name    = NULL;
  unsigned char* app_data = NULL;
  size_t app_size         = 0;
  size_t binary_size      = 0;

  app_name    = binary_names[id];
  app_data    = (uint8_t*)(uintptr_t)binaries[id];
  app_size    = binary_sizes[id];
  binary_size = actual_sizes[id];

  printf("[AppLoader] Requested to load %s!\n", app_name);

  int ret = libtock_app_loader_setup(app_size, app_setup_done_callback);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Error] Setup Failed: %d.\n", ret);
    return -1;
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
    return -1;
  }
  // wait on load done callback
  yield_for(&load_done);
  load_done = false;

  printf("[Success] Process created successfully.\n");

  return 0;
}

int uninstall_application(uint32_t id) {
  int ret4 = libtock_app_loader_unload(id, app_unload_done_callback);
  if (ret4 != RETURNCODE_SUCCESS) {
    printf("[Error] unload Failed: %d.\n", ret4);
    tock_exit(ret4);
  }
  yield_for(&unload_done);
  unload_done = false;

  printf("[AppLoader] Uninstalling app with identifier: %" PRIu32 "\n", um.app_handle);
  int ret5 = libtock_app_loader_uninstall_with_app_handle(um.app_handle, app_uninstall_done_callback);
  if  (ret5 != RETURNCODE_SUCCESS) {
    printf("[Error] Uninstall Failed: %d\n", ret5);
    tock_exit(ret5);
  }
  yield_for(&uninstall_done);
  uninstall_done = false;
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

      uint8_t app_id = buffer[1];
      int ret        = install_binary(app_id);
      buffer[0] = ret;
      ipc_notify_client(pid);
      break;

    case 3:
      // uninstall certain app
      if (len < 2) {
        // short id missing
        printf("[AppLoader] Returning on Command 0x03\n");
        return;
      }

      uint32_t short_id =
      ((uint32_t)buffer[1] << 24) |
      ((uint32_t)buffer[2] << 16) |
      ((uint32_t)buffer[3] <<  8) |
      ((uint32_t)buffer[4]);
      int ret1 = uninstall_application(short_id);
      buffer[0] = ret1;
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
