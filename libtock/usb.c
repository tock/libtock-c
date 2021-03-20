#include "usb.h"

int usb_exists(void) {
  return driver_exists(DRIVER_NUM_USB);
}

int usb_subscribe(subscribe_upcall upcall, void *ud) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_USB, 0, upcall, ud);
  if (sval.success) {
    return 0;
  } else {
    return tock_error_to_rcode(sval.error);
  }
}

int usb_enable_and_attach_async(void) {
  syscall_return_t com = command(DRIVER_NUM_USB, 1, 0, 0);
  if (com.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (com.type > TOCK_SYSCALL_SUCCESS) {
    // Returned an incorrect success code
    return TOCK_FAIL;
  } else {
    return tock_error_to_rcode(com.data[0]);
  }
}

struct data {
  bool fired;
  int status;
};

static void callback(int status,
                     __attribute__((unused)) int v1,
                     __attribute__((unused)) int v2,
                     void *data)
{
  struct data *d = data;
  d->fired  = true;
  d->status = status;
}

int usb_enable_and_attach(void)
{
  int status;

  struct data d = { .fired = false };

  if ((status = usb_subscribe(callback, (void *) &d)) != TOCK_SUCCESS) {
    return status;
  }

  if ((status = usb_enable_and_attach_async()) != TOCK_SUCCESS) {
    return status;
  }

  yield_for(&d.fired);
  return d.status;
}
