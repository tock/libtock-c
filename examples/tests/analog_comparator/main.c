#include <stdio.h>
#include <stdlib.h>

#include <analog_comparator.h>
#include <timer.h>
#include <tock.h>
static int channel;

static void ac_comparison_polling(uint8_t ac) {
  uint count = 0;
  while (1) {
    count++;
    bool result = ac_comparison(ac);
    printf("Try %d. Result = %d.\n", count, result);
    if (result == 1) {
      printf("This implies Vinp > Vinn!\n\n");
    }else {
      printf("This implies Vinp < Vinn!\n\n");
    }
    delay_ms(1000);
  }
}

// Callback for AC interrupts. Channel on which the interrupt is generated is
// passed through here.
static void ac_cb (__attribute__ ((unused)) int arg0,
                   __attribute__ ((unused)) int arg1,
                   __attribute__ ((unused)) int arg2,
                   __attribute__ ((unused)) void* userdata) {
  channel = arg0;
}

static void ac_comparison_interrupt(uint8_t ac) {
  // Enable AC interrupts
  ac_enable_interrupts(ac);

  // Set callback for AC interrupts
  ac_interrupt_callback(ac_cb, NULL);

  while (1) {
    yield();
    printf("Interrupt received on channel %d, Vinp > Vinn!\n", channel);
  }
}

int main(void) {
  printf("\nAnalog Comparator test application\n");

  if (!ac_exists()) {
    printf("Analog Comparator driver does not exist\n");
    exit(1);
  }
  printf("Analog Comparator driver exists with %d channels\n", ac_count());

  // Set mode according to which implementation you want.
  // mode = 0 --> polling comparison
  // mode = 1 --> interrupt-based comparison
  uint8_t mode = 1;

  // Choose a comparator, starting from index 0 and depending on the chip
  uint8_t ac = 1;

  switch (mode) {
    // Poll for a normal comparison every second and print the result
    case 0: ac_comparison_polling(ac); break;

    // Print for every interrupt received
    case 1: ac_comparison_interrupt(ac); break;

  }
  printf("\n");
  return 0;
}

