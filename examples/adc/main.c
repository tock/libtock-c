#include <adc.h>
#include <stdio.h>
#include <timer.h>

int main(void) {
  // Ask the kernel how many ADC channels are on this board.
  int num_adc = adc_channel_count();

  printf ("ADC Channels: %d\n", num_adc);

  while (true)
  {
    for (int channel = 0; channel < num_adc; channel++) {
      uint16_t value;
      if (adc_sample_sync (channel, &value) == TOCK_SUCCESS) {
        printf ("Channel %d: %d\n", channel, value);
      } else {
        printf ("Channel %d: error \n", channel);
      }
    }
    printf ("\n");
    delay_ms(1000);
  }
}
