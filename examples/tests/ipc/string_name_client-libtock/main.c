#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/ipc/ipc_registry_string_name.h>

static const char* SERVICE_NAME = "Test_Service";

static void ipc_discovery_complete(returncode_t ret, uint64_t ipc_id) {
  if (ret == RETURNCODE_SUCCESS) {
    uint32_t lower = (uint32_t)ipc_id;
    uint32_t upper = (uint32_t)(ipc_id >> 32);
    printf("CLIENT: Discovery complete. Succeeded! Found IPC ID: %08X%08X\n", (unsigned int)upper, (unsigned int)lower);
  } else if (ret == RETURNCODE_ENODEVICE) {
    printf("CLIENT: Discovery complete. Service not found.\n");
  } else {
    printf("CLIENT: Discovery complete. Failed with returncode: %d\n", ret);
  }
}

int main(void) {
  returncode_t ret = RETURNCODE_SUCCESS;
  printf("CLIENT: Starting client.\n");

  // Check that we can find the driver at all
  if (!libtock_ipc_registry_string_name_exists()) {
    printf("CLIENT: IPC Registry String Name capsule does not exist on this board.\n");
    return 1;
  } else {
    printf("CLIENT: IPC Registry String Name capsule exists.\n");
  }

  // Delay a bit
  libtocksync_alarm_delay_ms(1000);

  // Attempt to discover (this should fail as the server hasn't registered yet)
  printf("CLIENT: Discovering: \"%s\"\n", SERVICE_NAME);
  ret = libtock_ipc_registry_string_name_discover_service(ipc_discovery_complete, (uint8_t*)SERVICE_NAME,
                                                          strlen(SERVICE_NAME));
  if (ret != RETURNCODE_SUCCESS) {
    printf("CLIENT: Discovery failed. Returncode: %d\n", ret);
  } else {
    printf("CLIENT: Discovery started.\n");
  }

  // Delay for a bit
  libtocksync_alarm_delay_ms(5000);

  // Re-attempt to discover (this should now succeed)
  printf("CLIENT: Discovering: \"%s\"\n", SERVICE_NAME);
  ret = libtock_ipc_registry_string_name_discover_service(ipc_discovery_complete, (uint8_t*)SERVICE_NAME,
                                                          strlen(SERVICE_NAME));
  if (ret != RETURNCODE_SUCCESS) {
    printf("CLIENT: Discovery failed. Returncode: %d\n", ret);
  } else {
    printf("CLIENT: Discovery started.\n");
  }

  while (1) {
    yield();
  }
}
