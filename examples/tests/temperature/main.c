#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/sensors/temperature.h>

int main(void) {
  returncode_t ret;

  if (!libtocksync_temperature_exists()) {
    printf("[Temperature] No temperature sensor found.\n");
    exit(-1);
  }

  int temp;
  ret = libtocksync_temperature_read(&temp);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Temperature] Error reading temperature sensor.\n");
    printf("[Temperature] %s\n", tock_strrcode(ret));
    exit(-2);
  }

  int temp_ones       = temp / 100;
  int temp_hundredths = temp - (temp_ones * 100);
  printf("[Temperature] %d.%d deg C\n", temp_ones, temp_hundredths);

  return 0;
}
