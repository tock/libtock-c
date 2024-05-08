#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/display/screen.h>
#include <libtock/sensors/touch.h>

libtock_touch_event_t* multi_touch_buffer;

static void touch_event (int status, uint16_t x, uint16_t y) {
  switch (status) {
    case LIBTOCK_TOUCH_STATUS_PRESSED: {
      printf("pressed ");
      break;
    }
    case LIBTOCK_TOUCH_STATUS_RELEASED: {
      printf("released ");
      break;
    }
    case LIBTOCK_TOUCH_STATUS_MOVED: {
      printf("moved ");
      break;
    }
    default:
      printf("error ");
  }
  printf("(%d): %d y %d\n", status, x, y);
}

static void multi_touch_event (
  returncode_t ret __attribute__ ((unused)),
  int          num_touches,
  int          num_dropped __attribute__ ((unused)),
  int          num_nofit __attribute__ ((unused))) {
  for (int i = 0; i < num_touches; i++) {
    uint8_t id, status, x, y, size, pressure;
    libtock_touch_read_touch_from_buffer(multi_touch_buffer, i, &id, &status, &x, &y, &size, &pressure);
    printf("%d: ", id);
    switch (status) {
      case LIBTOCK_TOUCH_STATUS_PRESSED: {
        printf("pressed ");
        break;
      }
      case LIBTOCK_TOUCH_STATUS_RELEASED: {
        printf("released ");
        break;
      }
      case LIBTOCK_TOUCH_STATUS_MOVED: {
        printf("moved ");
        break;
      }
      default:
        printf("error ");
    }
    printf("(%d): %d y %d, ", status, x, y);
  }
  printf("\n");
  // ack the multi touch event and enable the next event
  libtock_touch_multi_touch_next();
}

int main(void) {
  int num_touches = 0;
  libtock_touch_get_number_of_touches(&num_touches);
  printf("Number of touches: %d\n", num_touches);
  if (num_touches == 0) {
    printf("No touch found\n");
  } else if (num_touches == 1) {
    // single touch
    libtock_touch_enable_single_touch(touch_event);
  } else {
    // multi touch
    libtock_touch_allocate_multi_touch_buffer(num_touches, &multi_touch_buffer);
    libtock_touch_enable_multi_touch(multi_touch_buffer, num_touches, multi_touch_event);
  }

  while (1) {
    yield();
  }
}
