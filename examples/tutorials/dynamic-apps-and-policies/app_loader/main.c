#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/kernel/app_loader.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/kernel/ipc.h>

#include "loadable_binaries.h"

#define FLASH_BUFFER_SIZE 512
#define RETURNCODE_SUCCESS 0

uint8_t app_id = 0;


/********************************
 * Function prototypes
 *********************************/
int install_binary(uint8_t id);
int uninstall_application(uint32_t short_id);
int write_app(double size, uint8_t binary[]);


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

  int ret = libtocksync_app_loader_setup(app_size);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Error] Setup Failed: %d.\n", ret);
    return -1;
  }

  printf("[Success] Setup successful. Writing app to flash.\n");
  size_t offset = 0;
  while (offset < binary_size) {
    size_t chunk_len = (binary_size - offset > FLASH_BUFFER_SIZE)
                        ? FLASH_BUFFER_SIZE
                        : binary_size - offset;

    int ret1 = libtocksync_app_loader_write(offset, chunk_len, &app_data[offset]);
    if (ret1 != RETURNCODE_SUCCESS) {
      printf("[Error] App flash write unsuccessful: %d.\n", ret1);
      return -1;
    }
    offset += chunk_len;
  }

  printf("[Success] Done writing app, finalizing.\n");
  int ret2 = libtocksync_app_loader_finalize();
  if (ret2 != RETURNCODE_SUCCESS) {
    printf("[Error] Failed to finalize new process binary.\n");
    return -1;
  }

  printf("[Success] App flashed successfully. Creating process now.\n");
  int ret3 = libtocksync_app_loader_load();
  if (ret3 != RETURNCODE_SUCCESS) {
    printf("[Error] Process creation failed: %d.\n", ret3);
    return -1;
  }

  printf("[Success] Process created successfully.\n");

  return 0;
}


int uninstall_application(uint32_t id) {
  uint32_t app_handle;
  int ret4 = libtocksync_app_loader_unload(id, &app_handle);
  if (ret4 != RETURNCODE_SUCCESS) {
    printf("[Error] unload Failed: %d.\n", ret4);
    return -1;
  }

  printf("[Success] Unloaded process.\n");

  printf("[AppLoader] Uninstalling app.\n");
  int ret5 = libtocksync_app_loader_uninstall_with_app_handle(app_handle);
  if  (ret5 != RETURNCODE_SUCCESS) {
    printf("[Error] Uninstall Failed: %d\n", ret5);
    return -1;
  }
  printf("[Success] Uninstalled app with identifier: %" PRIu32 "\n", app_handle);

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
        ((uint32_t)buffer[3] << 8) |
        ((uint32_t)buffer[4]);
      int ret1 = uninstall_application(short_id);
      buffer[0] = ret1;
      ipc_notify_client(pid);
      break;
  }
}


int main(void) {

  if (!libtocksync_app_loader_exists()) {
    printf("[Error] Failed to detect App Loader Driver!\n");
    return -1;
  }

  ipc_register_service_callback("app_loader", ipc_callback,
                                NULL);

  while (1) {
    yield();
  }
}
