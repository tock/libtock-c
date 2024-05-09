#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <timer.h>

#include <button.h>
#include <led.h>

#include <ipc.h>

#include <u8g2-tock.h>
#include <u8g2.h>

size_t sensor_svc_num = 0;

uint8_t local_temperature_setpoint = 22;
uint8_t global_temperature_setpoint = 0;
uint8_t prior_global_temperature_setpoint = 255;

int measured_temperature = 0;
int prior_measured_temperature = 0;

bool network_up = false;

// We use this variable as a buffer that is naturally aligned to the int
// alignment, and has an alignment >= its size.
uint8_t temperature_buffer[64] __attribute__((aligned(64)));

static void update_screen(void);
static int init_controller_ipc(void);

static void sensor_callback(__attribute__ ((unused)) int pid,
                            __attribute__ ((unused)) int len,
                            __attribute__ ((unused)) int arg2,
                            __attribute__ ((unused)) void* ud) {
  // update displayed measured temperature
  measured_temperature = *((int*) &temperature_buffer[0]);
  if(measured_temperature != prior_measured_temperature){
    prior_measured_temperature = measured_temperature;
    update_screen();
  }
}

static void button_callback(int                            btn_num,
                            int                            val,
                            __attribute__ ((unused)) int   arg2,
                            __attribute__ ((unused)) void *ud) {
  if (val == 1) {
    if (btn_num == 0 && local_temperature_setpoint < 35) {
      local_temperature_setpoint++;
    } else if (btn_num == 1 && local_temperature_setpoint > 0) {
      local_temperature_setpoint--;
    } else if (btn_num == 2) {
      local_temperature_setpoint = 22;
    }
  }
  update_screen();
}

int main(void) {
  int err = -1;

  init_controller_ipc();

  err = button_subscribe(button_callback, NULL);
  if (err < 0) return err;

  button_enable_interrupt(0);
  button_enable_interrupt(1);
  button_enable_interrupt(2);

  for(;;) {
    ipc_notify_service(sensor_svc_num);
    delay_ms(250);
  }
}

static int init_controller_ipc(void){
  int err = -1;
  int discover_retry_count = 0;
  int err_sensor = -1;

  while (err_sensor < 0 && discover_retry_count < 100) {
    err_sensor = ipc_discover("org.tockos.thread-tutorial.sensor", &sensor_svc_num);
    discover_retry_count++;
    if (err < 0) {
      delay_ms(10);
    }
  }

  if (err_sensor < 0) {
    printf("No sensor service\r\n");
    return -1;
  }

  printf("[controller] Discovered sensor service: %d\r\n", sensor_svc_num);

  ipc_register_client_callback(sensor_svc_num, sensor_callback, NULL);
  ipc_share(sensor_svc_num, &temperature_buffer, sizeof(temperature_buffer));

  return err;
}

static void update_screen(void) {
}
