#include "qdec.h"

/*struct data {
  bool fired;
  int qdec;
};

static struct data result = {.fired = false};

// Internal callback for faking synchronous reads
static void cb(int qdec,
               __attribute__ ((unused)) int unused,
               __attribute__ ((unused)) int unused1,
               void* ud) {
  struct data* data = (struct data*) ud;
  data->qdec  = qdec;
  data->fired = true;
}*/

bool qdec_exists(void) {
  return command(DRIVER_NUM_QDEC, 0, 0, 0) >= 0;
}

int qdec_enable(void) {
  return command(DRIVER_NUM_QDEC, 1, 1, 0);
}

int qdec_interrupt_enable() {
  return command(DRIVER_NUM_QDEC, 2, 1, 0);
}

int qdec_set_callback(subscribe_cb callback, void* callback_args) {
  return subscribe(DRIVER_NUM_QDEC, 0, callback, callback_args);
}

int qdec_displacement(void) {
  return command(DRIVER_NUM_QDEC, 3, 0, 0);
}

