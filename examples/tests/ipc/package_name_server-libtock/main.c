#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/ipc/ipc_registry_package_name.h>

static void ipc_registration_complete(returncode_t ret) {
  if (ret == RETURNCODE_SUCCESS) {
    printf("SERVER: Registration complete. Succeeded!\n");
  } else {
    printf("SERVER: Registration complete. Failed with returncode: %d\n", ret);
  }
}

int main(void) {
  printf("SERVER: Starting server.\n");

  // Check that we can find the driver at all
  if (!libtock_ipc_registry_package_name_exists()) {
    printf("SERVER: IPC Registry Package Name capsule does not exist on this board.\n");
    return 1;
  } else {
    printf("SERVER: IPC Registry Package Name capsule exists.\n");
  }

  // Delay a bit
  libtocksync_alarm_delay_ms(5000);

  // Attempt to register
  printf("SERVER: Registering with my package name (see Makefile).\n");
  returncode_t ret = libtock_ipc_registry_package_name_register_service(ipc_registration_complete);
  if (ret != RETURNCODE_SUCCESS) {
    printf("SERVER: Registration failed. Returncode: %d\n", ret);
  } else {
    printf("SERVER: Registration started.\n");
  }

  while (1) {
    yield();
  }
}
