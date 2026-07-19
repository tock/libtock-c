#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/ipc/syscalls/ipc_registry_string_name_syscalls.h>
#include <libtock/ipc/syscalls/ipc_synchronous_mailbox_syscalls.h>

// Register a service
static uint8_t string_name[20] = {0};

// Synchronous mailbox
static uint64_t client_ipc_id = 0;
static uint8_t incoming_request_data[6] = {0};
static uint8_t outgoing_response_data[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
static volatile bool request_received = false;


static void ipc_registry_string_name_upcall_registration_complete(int                            arg1,
                                                                  __attribute__ ((unused)) int   unused1,
                                                                  __attribute__ ((unused)) int   unused2,
                                                                  __attribute__ ((unused)) void* opaque) {
  printf("SERVER: Registration complete! Success?: %d\n", (bool)arg1);
}

static void ipc_synchronous_mailbox_upcall_request_waiting(int arg1, int arg2,
                                                          __attribute__ ((unused)) int arg3,
                                                          __attribute__ ((unused)) void* opaque) {
  client_ipc_id = (((uint64_t)arg3) << 32) | ((uint64_t)arg2);
  printf("SERVER: Request waiting! Client ipc_id: %ld\n", client_ipc_id);
  request_received = true;
}

int main(void) {
  printf("SERVER: Starting service\n");

  // Check that we can find the driver at all
  printf("SERVER: Registration Driver exists: %d\n", libtock_ipc_registry_string_name_driver_exists());
  libtocksync_alarm_delay_ms(3000);

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

  // Setup mailbox too
  returncode_t e = libtock_ipc_synchronous_mailbox_set_upcall_server_request_waiting(ipc_synchronous_mailbox_upcall_request_waiting, NULL);
  //libtock_ipc_synchronous_mailbox_command_server_get_next_request(&len, &incoming_ipc_id);
  printf("SERVER: Sync mailbox setup Returncodes: %d\n", e);
  yield();

  // Wait for request
  while(!request_received) {
    libtocksync_alarm_delay_ms(100);
  }

  // Receive request
  returncode_t f = libtock_ipc_synchronous_mailbox_set_allow_write_buffer(incoming_request_data, 6);
  uint32_t len = 0;
  uint64_t incoming_ipc_id = 0;
  returncode_t g = libtock_ipc_synchronous_mailbox_command_server_get_next_request(&len, &incoming_ipc_id);
  returncode_t h = libtock_ipc_synchronous_mailbox_set_allow_write_buffer(0, 0);
  printf("SERVER: Sync mailbox get request Returncodes: %d %d %d\n", f, g, h);
  printf("SERVER: Sync mailbox incoming request Data: ");
  for (int i=0; i<6; i++) {
    printf("%X ", incoming_request_data[i]);
  }
  printf("\n");

  // Send response
  libtocksync_alarm_delay_ms(3000);
  returncode_t j = libtock_ipc_synchronous_mailbox_set_allow_read_buffer(outgoing_response_data, 8);
  returncode_t k = libtock_ipc_synchronous_mailbox_command_server_send_response();
  printf("SERVER: Sent response. Returncodes: %d %d\n", j, k);

  while (1) {
    yield();
  }
}
