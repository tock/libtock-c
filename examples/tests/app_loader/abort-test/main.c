#include <math.h>
#include <stdio.h>
#include <string.h>

#include <libtock-sync/interface/console.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/kernel/app_loader.h>
#include <libtock/tock.h>

#include "loadable_binaries.h"

/******************************************************************************************************
* Callback Tracking Flags
******************************************************************************************************/
static bool setup_done     = false;     // to check if setup is done
static bool abort_done     = false;     // to check if write abort was successful
volatile bool abort_flag   = false;     // track if abort request was sent
uint32_t write_buffer_size = 4096;

/******************************************************************************************************
* Function Prototypes
******************************************************************************************************/
void abort_test(void);

/******************************************************************************************************
 * Callback functions
 *
 * 1. Set callback to initiate the dynamic app load process
 * 2. Set callback to call `abort()` with a timer
 *******************************************************************************************************/

static void app_setup_done_callback(__attribute__((unused)) int   arg0,
                                    __attribute__((unused)) int   arg1,
                                    __attribute__((unused)) int   arg2,
                                    __attribute__((unused)) void* ud) {
  setup_done = true;
}

static void app_abort_done_callback(__attribute__((unused)) int   arg0,
                                    __attribute__((unused)) int   arg1,
                                    __attribute__((unused)) int   arg2,
                                    __attribute__((unused)) void* ud) {
  abort_done = true;
}

static void abort_alarm_cb(__attribute__ ((unused)) uint32_t now,
                           __attribute__ ((unused)) uint32_t scheduled,
                           __attribute__ ((unused)) void*    opaque) {
  printf("[Timer] Triggering abort now.\n");
  abort_flag = true;
  int ret = libtock_app_loader_abort(app_abort_done_callback);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Error] Abort Failed: %d.\n", ret);
  }

  // wait on abort done callback
  yield_for(&abort_done);
  abort_done = false;
  printf("[Success] Abort Successful.\n");
}

/******************************************************************************************************
* Function to test the abort functionality of Tock's dynamic app loading feature set
*
* Takes callback function as argument
******************************************************************************************************/

void abort_test(void) {

  libtock_alarm_t abort_alarm;

  unsigned char* app_data = (uint8_t*)(uintptr_t)binaries[0];
  size_t app_size         = binary_sizes[0];
  size_t binary_size      = actual_sizes[0];

  int ret = libtock_app_loader_setup(app_size, app_setup_done_callback);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[Error] Setup Failed: %d.\n", ret);
    tock_exit(ret);
  }

  // wait on setup done callback
  yield_for(&setup_done);
  setup_done = false;

  libtock_alarm_in_ms(150, abort_alarm_cb, NULL, &abort_alarm);

  size_t offset = 0;
  while (offset < binary_size) {
    if (abort_flag) break;
    size_t chunk_len = (binary_size - offset > write_buffer_size)
                        ? write_buffer_size
                        : binary_size - offset;

    int ret1 = libtock_app_loader_write(offset, &app_data[offset], chunk_len);
    if (ret1 != RETURNCODE_SUCCESS) {
      printf("[Error] Chunk write failed at offset %zu\n", offset);
      break;
    }
    offset += chunk_len;
  }
}

/******************************************************************************************************
* Main
******************************************************************************************************/

int main(void) {
  printf("[Log] Simple test app to test abort functionality of dynamic process loading.\n");

  // check if app loader driver exists
  if (!libtock_app_loader_exists()) {
    printf("No App Loader driver!\n");
    return -1;
  }

  libtocksync_alarm_delay_ms(5000);

  printf("[Log] Initiating Abort Test.\n");

  abort_test();

  printf("[Log] Exiting Abort Test.\n");
  return 0;
}
