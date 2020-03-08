#include <gpio.h>
#include <timer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <console.h>

char pin_str[] = "Found pin number      \r\n";

static void nop(
  int a __attribute__((unused)),
  int b __attribute__((unused)),
  int c __attribute__((unused)),
  void* d __attribute__((unused))) {}

int main(void) {
  // search for pin PE09 (nucleo f429zi)
  // msb 4 bits, port E (4th port, staring from 0) - 0100
  // lsb 4 bits, pin 9 - 1001 
  int pin_number = gpio_get_pin_by_id (0b01001001);

  snprintf (pin_str, sizeof (pin_str)-1, "Found pin number %d\r\n", pin_number);
  
  putnstr_async(pin_str, strlen(pin_str), nop, NULL);

  if (pin_number > 0)
  {
    gpio_enable_output (pin_number);
    while (1)
    {
      gpio_set (pin_number);
      delay_ms (500);
      gpio_clear (pin_number);
      delay_ms (500);
    }
  }

  while (1)
  {

  }
}
