#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/ipc/syscalls/ipc_registry_string_name_syscalls.h>
#include <libtock/ipc/syscalls/ipc_synchronous_mailbox_syscalls.h>

// Discover a service
static uint8_t string_name[20] = {0};
static uint64_t server_ipc_id = 0;

// Synchronous mailbox
static uint8_t request_data[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xC0, 0xDE};
static uint8_t response_data[8] = {0};
static volatile bool response_received = false;

static void ipc_registry_string_name_upcall_new_registration(
  __attribute__ ((unused)) int   arg1,
  __attribute__ ((unused)) int   unused1,
  __attribute__ ((unused)) int   unused2,
  __attribute__ ((unused)) void* opaque) {
  printf("CLIENT: New registration occurred\n");
}

static void ipc_registry_string_name_upcall_discovery_complete(int arg1, int arg2, int arg3,
                                                               __attribute__ ((unused)) void* opaque) {
  server_ipc_id = (((uint64_t)arg3) << 32) | ((uint64_t)arg2);
  printf("CLIENT: Discovery complete! Returncode: %d Found server ipc_id: %ld\n", arg1, server_ipc_id);
}

static void ipc_synchronous_mailbox_upcall_response_received(int arg1, int arg2,
                                                            __attribute__ ((unused)) int arg3,
                                                            __attribute__ ((unused)) void* opaque) {
  printf("CLIENT: Response received! Returncode: %d Len: %d\n", arg1, arg2);
  response_received = true;
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
  libtocksync_alarm_delay_ms(3000);

  // Re-attempt to discover
  printf("CLIENT: Discovering: \"%s\"\n", name);
  c = libtock_ipc_registry_string_name_command_discover();
  printf("CLIENT: Re-Discovery Returncodes: %d\n", c);
  libtocksync_alarm_delay_ms(3000);

  // Attempt to send request
  returncode_t e = libtock_ipc_synchronous_mailbox_set_upcall_client_response_received(ipc_synchronous_mailbox_upcall_response_received, NULL);
  returncode_t f = libtock_ipc_synchronous_mailbox_set_allow_read_buffer(request_data, 6);
  returncode_t g = libtock_ipc_synchronous_mailbox_set_allow_write_buffer(response_data, 8);
  returncode_t h = libtock_ipc_synchronous_mailbox_command_client_send_request(server_ipc_id);
  printf("CLIENT: Sync mailbox request Returncodes: %d %d %d %d\n", e, f, g, h);
  yield();

  // Wait for response
  while(!response_received) {
    libtocksync_alarm_delay_ms(100);
  }

  // Attempt to read response
  libtock_ipc_synchronous_mailbox_set_allow_write_buffer(0, 0);
  printf("CLIENT: Sync mailbox response Data: ");
  for (int i=0; i<8; i++) {
    printf("%X ", response_data[i]);
  }
  printf("\n");

  while (1) {
    yield();
  }
}
