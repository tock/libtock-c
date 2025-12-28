#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/ipc/syscalls/ipc_registry_package_name_syscalls.h>

// Service name (specified in Makefile of service)
char service_name[] = "tbf_package_name_service";

static void ipc_registry_package_name_upcall_new_registration(
  __attribute__ ((unused)) int   arg1,
  __attribute__ ((unused)) int   unused1,
  __attribute__ ((unused)) int   unused2,
  __attribute__ ((unused)) void* opaque) {
  printf("CLIENT: New registration occurred\n");
}

static void ipc_registry_package_name_upcall_discovery_complete(int arg1, int arg2,
                                                                __attribute__ ((unused)) int unused1,
                                                                __attribute__ ((unused)) void* opaque) {
  printf("CLIENT: Discovery complete! Succeeded: %d Found processid: %d\n", (bool)arg1, (int)arg2);
}

int main(void) {
  printf("CLIENT: Starting client\n");

  // Check that we can find the driver at all
  printf("CLIENT: Discovery Driver exists: %d\n", libtock_ipc_registry_package_name_driver_exists());
  libtocksync_alarm_delay_ms(1000);

  // Request new registration notifications
  returncode_t d =
    libtock_ipc_registry_package_name_set_upcall_new_registration(ipc_registry_package_name_upcall_new_registration,
                                                                  NULL);
  printf("CLIENT: New registration notification returncodes: %d\n", d);

  // Attempt to discover
  printf("CLIENT: Discovering: \"%s\"\n", service_name);
  returncode_t a =
    libtock_ipc_registry_package_name_set_upcall_discovery_complete(ipc_registry_package_name_upcall_discovery_complete,
                                                                    NULL);
  returncode_t b = libtock_ipc_registry_package_name_set_allow_name(service_name, strlen(service_name));
  returncode_t c = libtock_ipc_registry_package_name_command_discover();
  printf("CLIENT: Discovery Returncodes: %d %d %d\n", a, b, c);
  libtocksync_alarm_delay_ms(5000);

  // Re-attempt to discover
  printf("CLIENT: Discovering: \"%s\"\n", service_name);
  c = libtock_ipc_registry_package_name_command_discover();
  printf("CLIENT: Re-Discovery Returncodes: %d\n", c);

  while (1) {
    yield();
  }
}
