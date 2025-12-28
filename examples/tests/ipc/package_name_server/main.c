#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/ipc/syscalls/ipc_registry_package_name_syscalls.h>


static void ipc_registry_package_name_upcall_registration_complete(int                            arg1,
                                                                   __attribute__ ((unused)) int   unused1,
                                                                   __attribute__ ((unused)) int   unused2,
                                                                   __attribute__ ((unused)) void* opaque) {
  printf("SERVER: Registration complete! Success?: %d\n", (bool)arg1);
}

int main(void) {
  printf("SERVER: Starting service\n");

  // Check that we can find the driver at all
  printf("SERVER: Registration Driver exists: %d\n", libtock_ipc_registry_package_name_driver_exists());
  libtocksync_alarm_delay_ms(5000);

  // Attempt to register
  printf("SERVER: Registering with my package name (see Makefile)\n");
  returncode_t a =
    libtock_ipc_registry_package_name_set_upcall_registration_complete(
      ipc_registry_package_name_upcall_registration_complete, NULL);
  returncode_t b = libtock_ipc_registry_package_name_command_register();
  printf("SERVER: Registration Returncodes: %d %d\n", a, b);

  while (1) {
    yield();
  }
}
