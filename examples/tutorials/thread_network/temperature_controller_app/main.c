#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <timer.h>

#include <button.h>
#include <led.h>

#include <ipc.h>

size_t sensor_svc_num = 0;
size_t openthread_svc_num = 1;

uint8_t temperature = 22;

// We use this variable as a buffer that is naturally aligned to the int
// alignment, and has an alignment >= its size.
uint8_t temperature_buffer[64] __attribute__((aligned(64)));
uint8_t openthread_buffer[64] __attribute__((aligned(64)));

static void sensor_callback(__attribute__ ((unused)) int pid,
                            __attribute__ ((unused)) int len,
                            __attribute__ ((unused)) int arg2, void* ud) {
  bool *called = (bool*) ud;
  *called = true;
}

static void openthread_callback(__attribute__ ((unused)) int pid,
                            __attribute__ ((unused)) int len,
                            __attribute__ ((unused)) int arg2, void* ud) {
  bool *called = (bool*) ud;
  *called = true;
}

static void button_callback(int                            btn_num,
                            int                            val,
                            __attribute__ ((unused)) int   arg2,
                            __attribute__ ((unused)) void *ud) {
  if (val == 1) {
    if (btn_num == 0) {
      temperature++;
    } else if (btn_num == 1) {
      temperature--;
    } else if (btn_num == 2) {
      temperature = 22;
    }
  }
}

int main(void) {
  int err = -1;
  int discover_retry_count = 0;
  bool ipc_sensor_called = false;
  bool ipc_openthread_called = false;


    err = button_subscribe(button_callback, NULL);
  if (err < 0) return err;

button_enable_interrupt(0);
button_enable_interrupt(1);
button_enable_interrupt(2);

  int err_sensor = -1;
  int err_openthread = -1;
  while (err_sensor < 0 && err_openthread < 0 && discover_retry_count < 100) {
    err_sensor = ipc_discover("org.tockos.thread-tutorial.sensor", &sensor_svc_num);
    err_openthread = ipc_discover("org.tockos.thread-tutorial.openthread", &openthread_svc_num);
    discover_retry_count++;
    if (err < 0) {
      delay_ms(10);
    }
  }

  if (err_sensor < 0) {
    printf("No sensor service\r\n");
    return -1;
  }

    if (err_openthread < 0) {
    printf("No openthread service\r\n");
    return -1;
  }

  printf("[controller] Discovered sensor service: %d\r\n", sensor_svc_num);
  printf("[controller] Discovered openthread service: %d\r\n", openthread_svc_num);

  ipc_register_client_callback(sensor_svc_num, sensor_callback, &ipc_sensor_called);
  ipc_register_client_callback(openthread_svc_num, openthread_callback, &ipc_openthread_called);

  //rb->length = snprintf(rb->buf, sizeof(rb->buf), "Hello World!");
  ipc_share(sensor_svc_num, &temperature_buffer, sizeof(temperature_buffer));

  ipc_share(openthread_svc_num, &openthread_buffer, sizeof(openthread_buffer));

  int prior_temperature = 0;
  int prior_global_avg = 0;
  uint8_t prior_setpoint = 0;

  while (1) {
    // puts("[controller] Requesting temperature readout.");
    ipc_sensor_called = false;
    ipc_notify_service(sensor_svc_num);
    ipc_notify_service(openthread_svc_num);
    yield_for(&ipc_sensor_called || &ipc_openthread_called);

    int read_temp = *((int*) &temperature_buffer[0]);
    int global_avg_openthread = *((int*) &openthread_buffer[0]);
    if (prior_temperature != read_temp || prior_setpoint != temperature
     || prior_global_avg != global_avg_openthread) {
      puts("\n");
      puts("====================================\n");
      printf("Temperature reading: %i\r\n", read_temp);
      printf("Preferred setpoint: %d\r\n", temperature);
      printf("Global setpoint: %d\r\n", global_avg_openthread);
      puts("====================================\r\n");
    }

    prior_temperature = read_temp;
    prior_setpoint = temperature;

    delay_ms(250);
  }
}
