#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <adc.h>
#include <timer.h>
#include <gpio.h>
#include <ipc.h>
#include <tock.h>


struct sensor_client {
  int pid;
  uint8_t* buffer;
};

struct sensor_client clients[10];
int client_count = 0;

static void ipc_callback(int pid, int len, int buf, __attribute__ ((unused)) void* ud) {
  uint8_t* buffer = (uint8_t*) buf;


  if (client_count == 10 || len < 4) return;

  clients[client_count].pid = pid;
  clients[client_count].buffer = buffer;
  client_count += 1;

}




static uint32_t take_measurement(int reference_voltage) {
  uint16_t samples[30];
  int err = adc_sample_buffer_sync(0, 25, samples, 30);
  if (err != RETURNCODE_SUCCESS) {
    printf("Error sampling ADC: %d\n", err);
    return -1;
  }

  for (int i=0; i<30; i++) {
    printf("sample: %i\n", samples[i]);
  }

  uint32_t total = 0;
  for (int i=0; i<30; i++) {
    total += samples[i];
  }

  uint32_t average = total / 30;
  uint32_t voltage_mv = (average * reference_voltage) / ((1<<16)-1);

  return voltage_mv;
}


int main(void) {
  printf("[Soil Moisture] Sensor App\n");

  // Check if ADC driver exists.
  if (!adc_exists()) {
    printf("No ADC driver!\n");
    return -1;
  }

  ipc_register_service_callback("soil.moisture.sensor", ipc_callback, NULL);

  int reference_voltage = adc_get_reference_voltage();
  if (reference_voltage > 0) {
    printf("ADC reference voltage %d.%dV\n", reference_voltage / 1000, reference_voltage % 1000);
  } else {
    reference_voltage = 3300;
    printf("ADC no reference voltage, assuming 3.3V\n");
  }

  gpio_set(0);


  uint32_t voltage_mv = take_measurement(reference_voltage);

  printf("voltage: %lu\n", voltage_mv);
  printf("voltage %ld.%ldV\n", voltage_mv / 1000, voltage_mv % 1000);

  return 0;
}
