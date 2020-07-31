#include <screen.h>
#include <touch.h>
#include <stdlib.h>
#include <timer.h>

static void tocuh_event (int status, int x, int y, void *ud __attribute__ ((unused))) {
  switch (status) {
    case TOUCH_STATUS_PRESSED: {
      printf ("pressed ");
      break;
    }
    case TOUCH_STATUS_RELEASED: {
      printf ("released ");
      break;
    }
    case TOUCH_STATUS_MOVED: {
      printf ("moved ");
      break;
    }
    default:
      printf ("error ");
  }
  printf ("%d x %d y %d\n", status, x, y);
}

int main(void) {
  single_touch_set_callback (tocuh_event, NULL);
  return 0;
}
