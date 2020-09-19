#include <led.h>
#include <qdec.h>
#include <timer.h>
#include <tock.h>

static void qdec_callback(int qdec_num,
                          __attribute__ ((unused)) int val,
                          __attribute__ ((unused)) int arg2,
                          __attribute__ ((unused)) void *ud) {
  printf("QDEC Absolute Position: %d\n", qdec_num);
}

int main(void) {
  printf("Starting The [QDEC] Test App for Callbacks\n");
  if (!qdec_exists()) {
    printf("No QDEC driver\n");
    return -1;
  }
  qdec_enable();
  if (qdec_subscribe(qdec_callback, NULL) != 0) {
    printf("Can't subscribe  to interrupt results!");
    return -1;
  }
  if (!qdec_interrupt_enable()) {
    printf("Can't enable interrupts!\n");
    return -1;
  }
  printf("QDEC has been fully initiated!\n");
  return 0;
}
