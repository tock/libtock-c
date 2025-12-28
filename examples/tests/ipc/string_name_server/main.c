#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/ipc/syscalls/ipc_registry_string_name_syscalls.h>

// Register a service
uint8_t string_name[20] = {0};


static void ipc_registry_string_name_upcall_registration_complete(int                            arg1,
                                                                  __attribute__ ((unused)) int   unused1,
                                                                  __attribute__ ((unused)) int   unused2,
                                                                  __attribute__ ((unused)) void* opaque) {
  printf("SERVER: Registration complete! Success?: %d\n", (bool)arg1);
}

int main(void) {
  printf("SERVER: Starting service\n");

  // Check that we can find the driver at all
  printf("SERVER: Registration Driver exists: %d\n", libtock_ipc_registry_string_name_driver_exists());
  libtocksync_alarm_delay_ms(5000);

  // Create string name
  const char* name = "Test_Service";
  for (int i = 0; i < 20 && name[i] != '\0'; i++) {
    string_name[i] = name[i];
  }

  // Attempt to register
  printf("SERVER: Registering as: \"%s\"\n", name);
  returncode_t a =
    libtock_ipc_registry_string_name_set_upcall_registration_complete(
      ipc_registry_string_name_upcall_registration_complete, NULL);
  returncode_t b = libtock_ipc_registry_string_name_set_allow_name(string_name, 20);
  returncode_t c = libtock_ipc_registry_string_name_command_register();
  printf("SERVER: Registration Returncodes: %d %d %d\n", a, b, c);

  while (1) {
    yield();
  }
}
