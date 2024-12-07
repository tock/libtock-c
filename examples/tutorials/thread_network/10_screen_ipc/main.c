#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <libtock/interface/button.h>
#include <libtock/kernel/ipc.h>
#include <libtock/services/alarm.h>
#include <libtock-sync/services/alarm.h>

#include <u8g2.h>
#include <u8g2-tock.h>

// Global reference to the u8g2 context.
u8g2_t u8g2;

// Helper method to update and format u8g2 screen.
static void update_screen(void);

uint8_t global_temperature_setpoint = 0;
uint8_t local_temperature_setpoint  = 22;
int measured_temperature        = 0;

uint8_t prior_global_temperature_setpoint = 255;
uint8_t prior_local_temperature_setpoint = 255;
int prior_measured_temperature = 0;

bool network_up = false;

bool callback_event = false;

libtock_alarm_t read_temperature_timer;
libtock_alarm_t network_timer;

// Helper method to initialize the screen.
static int init_controller_ipc(void);
size_t sensor_svc_num     = 0;
size_t openthread_svc_num = 0;

// We use this variable as a buffer that is naturally aligned to the int
// alignment, and has an alignment >= its size.
uint8_t temperature_buffer[64] __attribute__((aligned(64)));
uint8_t openthread_buffer[64] __attribute__((aligned(64)));


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
}

static void read_temperature_timer_callback(__attribute__ ((unused)) uint32_t now,
                                            __attribute__ ((unused)) uint32_t scheduled,
                                            __attribute__ ((unused)) void*    opaque) {
  // Request a new temperature reading from the sensor:
  ipc_notify_service(sensor_svc_num);
}

static void update_network_timer_callback(__attribute__ ((unused)) uint32_t now,
                                          __attribute__ ((unused)) uint32_t scheduled,
                                          __attribute__ ((unused)) void*    opaque) {
  openthread_buffer[0] = local_temperature_setpoint;
  ipc_notify_service(openthread_svc_num);
  libtock_alarm_in_ms(500, update_network_timer_callback, NULL, &network_timer);

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

static void openthread_callback(__attribute__ ((unused)) int   pid,
                                __attribute__ ((unused)) int   len,
                                __attribute__ ((unused)) int   arg2,
                                __attribute__ ((unused)) void* ud) {
  network_up = true;

  // update setpoint temperature
  global_temperature_setpoint = *((int*) &openthread_buffer[0]);

  // Indicate that we have received a callback.
  callback_event = true;

}


int main(void) {
  // Init u8g2
  u8g2_tock_init(&u8g2);
  u8g2_SetFont(&u8g2, u8g2_font_profont12_tr);
  u8g2_SetFontPosTop(&u8g2);

  init_controller_ipc();

  // Enable buttons
  for (int i = 0; i < 4; i++){
    libtock_button_notify_on_press(i, button_callback);
  }

  ipc_notify_service(sensor_svc_num);
  libtock_alarm_in_ms(500, update_network_timer_callback, NULL, &network_timer);

  for(;;) {
    callback_event = false;
    yield_for(&callback_event);
    
    if (measured_temperature != prior_measured_temperature
        || global_temperature_setpoint != prior_global_temperature_setpoint
        || local_temperature_setpoint != prior_local_temperature_setpoint)
    {
       prior_measured_temperature        = measured_temperature;
       prior_global_temperature_setpoint = global_temperature_setpoint;
       prior_local_temperature_setpoint  = local_temperature_setpoint;
       update_screen();
    }

  }  
}

static void update_screen(void) {
  char temperature_set_point_str[35];
  char temperature_global_set_point_str[35];
  char temperature_current_measure_str[35];
  
  // Format the buffers to be written.
  sprintf(temperature_set_point_str,
          "Set Point: %d", 
	  local_temperature_setpoint);
  
  if(network_up) {
    sprintf(temperature_global_set_point_str, 
            "Global Set Point: %d",
       	    global_temperature_setpoint);
  } else {
    sprintf(temperature_global_set_point_str,
            "Global Set Point: N/A");
  }

  uint8_t whole_temp = measured_temperature / 100;
  uint8_t decimal_temp = measured_temperature % 100;

  sprintf(temperature_current_measure_str, 
	  "Measured Temp: %d.%d",
	  whole_temp,
	  decimal_temp );

  // Use u8g2 library to draw each string.
  u8g2_ClearBuffer(&u8g2);
  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_DrawStr(&u8g2, 0, 0, temperature_set_point_str);
  u8g2_DrawStr(&u8g2, 0, 25, temperature_global_set_point_str);
  u8g2_DrawStr(&u8g2, 0, 50, temperature_current_measure_str);
  u8g2_SendBuffer(&u8g2);
}

static int init_controller_ipc(void) {
  int err = -1;
  int discover_retry_count = 0;
  int err_sensor     = -1;
  int err_openthread = -1;

  while (err_sensor < 0 && err_openthread < 0 && discover_retry_count < 100) {
    err_sensor     = ipc_discover("org.tockos.thread-tutorial.sensor", &sensor_svc_num);
    err_openthread = ipc_discover("org.tockos.thread-tutorial.openthread", &openthread_svc_num);
    discover_retry_count++;
    if (err < 0) {
      libtocksync_alarm_delay_ms(10);
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

  ipc_register_client_callback(sensor_svc_num, sensor_callback, NULL);
  ipc_register_client_callback(openthread_svc_num, openthread_callback, NULL);

  ipc_share(sensor_svc_num, &temperature_buffer, sizeof(temperature_buffer));
  ipc_share(openthread_svc_num, &openthread_buffer, sizeof(openthread_buffer));

  return err;
}

