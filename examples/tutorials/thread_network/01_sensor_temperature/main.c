#include <stdio.h>

#include <libtock-sync/sensors/temperature.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/kernel/ipc.h>
#include <libtock/tock.h>

static int current_temperature = 0;

int main(void) {
  // Measure temperature -- returned in the form 2200 => 22C
  libtocksync_temperature_read(&current_temperature);

  // Convert temperature
  int whole_degree = current_temperature / 100;
  int decimal_degree = current_temperature % 100;
  
  printf("Hello World, the temperature is: %i.%i\r\n", whole_degree, decimal_degree);
  
  return 0;
}
