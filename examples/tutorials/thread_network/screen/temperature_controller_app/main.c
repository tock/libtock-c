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

u8g2_t u8g2;

size_t sensor_svc_num = 0;
size_t openthread_svc_num = 1;

uint8_t local_temperature_setpoint = 22;
uint8_t global_temperature_setpoint = 0;
int measured_temperature = 0;

bool network_up = false;

// We use this variable as a buffer that is naturally aligned to the int
// alignment, and has an alignment >= its size.
uint8_t temperature_buffer[64] __attribute__((aligned(64)));
uint8_t openthread_buffer[64] __attribute__((aligned(64)));

static void update_screen(void);
static int init_controller_ipc(void);

static void sensor_callback(__attribute__ ((unused)) int pid,
                            __attribute__ ((unused)) int len,
                            __attribute__ ((unused)) int arg2,
                            __attribute__ ((unused)) void* ud) {
  // update displayed measured temperature
  measured_temperature = *((int*) &temperature_buffer[0]);
  update_screen();
}

static void openthread_callback(                     int network_status,
                            __attribute__ ((unused)) int len,
                            __attribute__ ((unused)) int arg2,
                            __attribute__ ((unused)) void* ud) {
  network_up = network_status;
  // update displayed setpoint temperature
  global_temperature_setpoint = *((int*) &openthread_buffer[0]);
  update_screen();
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
    u8g2_tock_init(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_profont12_tr);
    u8g2_SetFontPosTop(&u8g2);

    init_controller_ipc();

  int err = -1;

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

  ipc_register_client_callback(sensor_svc_num, sensor_callback, NULL);
  ipc_register_client_callback(openthread_svc_num, openthread_callback, NULL);

  //rb->length = snprintf(rb->buf, sizeof(rb->buf), "Hello World!");
  ipc_share(sensor_svc_num, &temperature_buffer, sizeof(temperature_buffer));

  ipc_share(openthread_svc_num, &openthread_buffer, sizeof(openthread_buffer));


  return err;
}

static void update_screen(void) {
  char temperature_set_point_str [25];
  char temperature_global_set_point_str [25];
  char temperature_current_measure_str [25];
  sprintf(temperature_set_point_str, "Set Point: %d", local_temperature_setpoint);
  if (network_up) {
    sprintf(temperature_global_set_point_str, "Global Set Point: %d", global_temperature_setpoint);
  } else {
    sprintf(temperature_global_set_point_str, "Global Set Point: N/A");
  }
  sprintf(temperature_current_measure_str, "Measured Temp: %d", measured_temperature);

  u8g2_ClearBuffer(&u8g2);
  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_DrawStr(&u8g2, 0, 0, temperature_set_point_str);
  u8g2_DrawStr(&u8g2, 0, 25, temperature_global_set_point_str);
  u8g2_DrawStr(&u8g2, 0, 50, temperature_current_measure_str);
  u8g2_SendBuffer(&u8g2);
}