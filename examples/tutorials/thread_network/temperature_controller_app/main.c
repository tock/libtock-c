#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <timer.h>

#include <ipc.h>

size_t sensor_svc_num = 0;

// We use this variable as a buffer that is naturally aligned to the int
// alignment, and has an alignment >= its size.
uint8_t temperature_buffer[64] __attribute__((aligned(64)));


static void sensor_callback(__attribute__ ((unused)) int pid,
                            __attribute__ ((unused)) int len,
                            __attribute__ ((unused)) int arg2, void* ud) {
  bool *called = (bool*) ud;
  *called = true;
}

int main(void) {
  int err = -1;
  int discover_retry_count = 0;
  bool ipc_called = false;

  while (err < 0 && discover_retry_count < 100) {
    err = ipc_discover("org.tockos.thread-tutorial.sensor", &sensor_svc_num);
    discover_retry_count++;
    if (err < 0) {
      delay_ms(10);
    }
  }

  if (err < 0) {
    printf("No sensor service\r\n");
    return -1;
  }

  printf("[controller] Discovered sensor service: %d\r\n", sensor_svc_num);

  ipc_register_client_callback(sensor_svc_num, sensor_callback, &ipc_called);

  //rb->length = snprintf(rb->buf, sizeof(rb->buf), "Hello World!");
  ipc_share(sensor_svc_num, &temperature_buffer, sizeof(temperature_buffer));


  while (1) {
    puts("[controller] Requesting temperature readout.");
    ipc_called = false;
    ipc_notify_service(sensor_svc_num);
    yield_for(&ipc_called);
    printf("[controller] Received IPC response, temperature: %d\r\n", *((int*) &temperature_buffer[0]));

    delay_ms(1000);
  }
}
