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
  int ret1 = write_app(binary_size, app_data);
  if (ret1 != RETURNCODE_SUCCESS) {
    printf("[Error] App flash write unsuccessful: %d.\n", ret1);
    return -1;
  }

  printf("[Success] App flashed successfully. Creating process now.\n");
  int ret2 = libtocksync_app_loader_load();
  if (ret2 != RETURNCODE_SUCCESS) {
    printf("[Error] Process creation failed: %d.\n", ret2);
    return -1;
  }

  printf("[Success] Process created successfully.\n");

  return 0;
}

/******************************************************************************************************
*
* Function to write the app into the flash
*
* Takes app size and the app binary as arguments
******************************************************************************************************/

int write_app(double size, uint8_t binary[]) {

  uint32_t write_count = 0;
  uint8_t write_buffer[FLASH_BUFFER_SIZE];
  uint32_t flash_offset = 0;

  // This value can be changed to different sizes
  // to mimic different bus widths.
  uint32_t write_buffer_size = FLASH_BUFFER_SIZE;

  write_count = (size + write_buffer_size - 1) / write_buffer_size;

  for (uint32_t offset = 0; offset < write_count; offset++) {

    memset(write_buffer, 0, write_buffer_size);
    // copy binary to write buffer
    flash_offset = (offset * write_buffer_size);
    size_t bytes_left = size - flash_offset;
    size_t chunk      = bytes_left < write_buffer_size ? bytes_left : write_buffer_size;
    memcpy(write_buffer, &binary[write_buffer_size * offset], chunk);
    int ret1 = libtocksync_app_loader_write(flash_offset, write_buffer_size, write_buffer, write_buffer_size);
    if (ret1 != RETURNCODE_SUCCESS) {
      printf("[Error] Failed writing data to flash at address: 0x%lx\n", flash_offset);
      printf("[Error] Error nature: %d\n", ret1);
      return -1;
    }
  }

  // Now that we are done writing the binary, we ask the kernel to finalize it.
  printf("Done writing app, finalizing.\n");
  int ret2 = libtocksync_app_loader_finalize();
  if (ret2 != RETURNCODE_SUCCESS) {
    printf("[Error] Failed to finalize new process binary.\n");
    return -1;
  }

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
