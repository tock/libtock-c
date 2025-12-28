#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/ipc/syscalls/ipc_registry_string_name_syscalls.h>

// Discover a service
uint8_t string_name[20] = {0};

static void ipc_registry_string_name_upcall_new_registration(
  __attribute__ ((unused)) int   arg1,
  __attribute__ ((unused)) int   unused1,
  __attribute__ ((unused)) int   unused2,
  __attribute__ ((unused)) void* opaque) {
  printf("CLIENT: New registration occurred\n");
}

static void ipc_registry_string_name_upcall_discovery_complete(int arg1, int arg2,
                                                               __attribute__ ((unused)) int unused1,
                                                               __attribute__ ((unused)) void* opaque) {
  printf("CLIENT: Discovery complete! Succeeded: %d Found processid: %d\n", (bool)arg1, (int)arg2);
}

int main(void) {
  printf("CLIENT: Starting client\n");

  // Check that we can find the driver at all
  printf("CLIENT: Discovery Driver exists: %d\n", libtock_ipc_registry_string_name_driver_exists());
  libtocksync_alarm_delay_ms(1000);

  // Request new registration notifications
  returncode_t d =
    libtock_ipc_registry_string_name_set_upcall_new_registration(ipc_registry_string_name_upcall_new_registration,
                                                                 NULL);
  printf("CLIENT: New registration notification returncodes: %d\n", d);

  // Create string name
  const char* name = "Test_Service";
  for (int i = 0; i < 20 && name[i] != '\0'; i++) {
    string_name[i] = name[i];
  }

  // Attempt to discover
  printf("CLIENT: Discovering: \"%s\"\n", name);
  returncode_t a =
    libtock_ipc_registry_string_name_set_upcall_discovery_complete(ipc_registry_string_name_upcall_discovery_complete,
                                                                   NULL);
  returncode_t b = libtock_ipc_registry_string_name_set_allow_name(string_name, 20);
  returncode_t c = libtock_ipc_registry_string_name_command_discover();
  printf("CLIENT: Discovery Returncodes: %d %d %d\n", a, b, c);
  libtocksync_alarm_delay_ms(5000);

  // Re-attempt to discover
  printf("CLIENT: Discovering: \"%s\"\n", name);
  c = libtock_ipc_registry_string_name_command_discover();
  printf("CLIENT: Re-Discovery Returncodes: %d\n", c);

  while (1) {
    yield();
  }
}
