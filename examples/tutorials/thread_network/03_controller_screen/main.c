#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <libtock/interface/button.h>
#include <libtock/interface/led.h>
#include <libtock/kernel/ipc.h>
#include <libtock/services/alarm.h>

#include <libtock-sync/services/alarm.h>

size_t sensor_svc_num = 0;

uint8_t global_temperature_setpoint       = 0;
uint8_t prior_global_temperature_setpoint = 255;

uint8_t local_temperature_setpoint       = 22;
uint8_t prior_local_temperature_setpoint = 255;

int measured_temperature       = 0;
int prior_measured_temperature = 0;

// Callback event indicator
bool callback_event = false;

libtock_alarm_t read_temperature_timer;

// We use this variable as a buffer that is naturally aligned to the int
// alignment, and has an alignment >= its size.
uint8_t temperature_buffer[64] __attribute__((aligned(64)));

static void update_screen(void);
static int init_controller_ipc(void);


static void read_temperature_timer_callback(__attribute__ ((unused)) uint32_t now,
                                            __attribute__ ((unused)) uint32_t scheduled,
                                            __attribute__ ((unused)) void*    opaque) {
  // Request a new temperature reading from the sensor:
  ipc_notify_service(sensor_svc_num);
}


static void sensor_callback(__attribute__ ((unused)) int   pid,
                            __attribute__ ((unused)) int   len,
                            __attribute__ ((unused)) int   arg2,
                            __attribute__ ((unused)) void* ud) {
  // update measured temperature
  measured_temperature = *((int*) &temperature_buffer[0]);

  // Indicate that we have received a callback.
  callback_event = true;

  // Request a new temperature reading in 250ms:
  libtock_alarm_in_ms(250, read_temperature_timer_callback, NULL, &read_temperature_timer);
}

static void button_callback(returncode_t ret,
                            int          btn_num,
                            bool         pressed) {
  if (ret != RETURNCODE_SUCCESS) return;

  if (pressed) {
    if (btn_num == 0 && local_temperature_setpoint < 35) {
      local_temperature_setpoint++;
    } else if (btn_num == 1 && local_temperature_setpoint > 0) {
      local_temperature_setpoint--;
    } else if (btn_num == 2) {
      local_temperature_setpoint = 22;
    }
  }

  // Indicate that we have received a callback.
  callback_event = true;

  return;
}

int main(void) {
  int err;
  int i;

  init_controller_ipc();

  // Enable buttons
  for (i = 0; i < 3; i++) {
    err = libtock_button_notify_on_press(i, button_callback);
    if (err < 0) return err;
  }

  ipc_notify_service(sensor_svc_num);

  for ( ;;) {
    callback_event = false;
    yield_for(&callback_event);

    if (measured_temperature != prior_measured_temperature
        || global_temperature_setpoint != prior_global_temperature_setpoint
        || local_temperature_setpoint != prior_local_temperature_setpoint) {
      prior_measured_temperature        = measured_temperature;
      prior_global_temperature_setpoint = global_temperature_setpoint;
      prior_local_temperature_setpoint  = local_temperature_setpoint;
      update_screen();
    }
  }
}

static int init_controller_ipc(void) {
  int err = -1;
  int discover_retry_count = 0;
  int err_sensor = -1;

  while (err_sensor < 0 && discover_retry_count < 100) {
    err_sensor = ipc_discover("org.tockos.thread-tutorial.sensor", &sensor_svc_num);
    discover_retry_count++;
    if (err < 0) {
      libtocksync_alarm_delay_ms(10);
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
  printf("[controller] TODO: update screen! Measured temperature: %d.%02d\r\n",
         measured_temperature / 100, measured_temperature % 100);
}
