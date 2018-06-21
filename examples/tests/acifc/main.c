#include <stdio.h>
#include <stdlib.h>

#include <analog_comparator.h>
#include <timer.h>
#include <tock.h>

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

static void ac_window_comparison_polling(uint8_t window) {
  uint count = 0;
    while (1) {
      count++;
      bool result = ac_window_comparison(window);
      printf("Try %d. Result = %d.\n", count, result);
      if (result == 1) {
        printf("This implies Vacbn_x+1 < Vcommon < Vacap_x!\n\n");
      }else {
        printf("This implies Vcommon < Vacan_x+1 or Vcommon > Vacap_x\n\n");
      }
      delay_ms(1000);
  }
}

// Callback for AC interrupts. Currently empty, arguments could be sent.
static void ac_cb (__attribute__ ((unused)) int arg0,
                   __attribute__ ((unused)) int arg1,
                   __attribute__ ((unused)) int arg2,
                   __attribute__ ((unused)) void* userdata) {}

static void ac_comparison_interrupt(uint8_t ac) {
  // Enable AC interrupts
  ac_enable_interrupts(ac);

  // Set callback for AC interrupts
  ac_interrupt_callback(ac_cb, NULL);

  while (1) {
    yield();
    printf("ACIFC Interrupt, Vinp > Vinn !\n");
  }
}

int main(void) {
  printf("\nAnalog Comparator test application\n");

  if (!ac_exists()) {
    printf("Analog Comparator driver does not exist\n");
    exit(1);
  }
  // Set mode according to which implementation you want
  uint8_t mode = 2;

  // Choose a comparator in case you want to do a comparison of two input values. For the hail, there are two comparators. ac = 0
  // corresponds to PA06 and PA07, whereas ac = 1 corresponds to PB02 and PB03.
  // On the hail these are the pins DAC and WKP, and A2 and A3 respectively.
  uint8_t ac = 1;

  // Choose a window in case you want to do a comparison of three input values. For the hail, there is only one window. For imix, there
  // are two (0 and 1).
  uint8_t window = 1;

  // Initialize the ACs by enabling some basic necessities
  ac_enable();

  switch (mode) {
    // Poll for a normal comparison every second and print the result
    case 0: ac_comparison_polling(ac); break;

    // Poll for a window comparison every second and print the result
    case 1: ac_window_comparison_polling(window); break;

    // Print for every interrupt received
    case 2: ac_comparison_interrupt(ac); break;

  }
  printf("\n");
  return 0;
}

