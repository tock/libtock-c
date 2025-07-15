// Hardware Root of Trust (HWRoT) Hardfault Driver Application
//
// When selected by the main screen HWRoT Demo application, calls a command
// on a driver which is intended to hardfault all applications.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock/kernel/ipc.h>
#include <libtock/tock.h>

#define LOG_WIDTH      32

bool started          = false;
bool log_done         = false;
size_t screen_service = -1;
char log_buf[LOG_WIDTH] __attribute__((aligned(LOG_WIDTH)));

const char SCREEN_SERVICE_NAME[] = "org.tockos.tutorials.attestation.screen";

static void ipc_callback(__attribute__ ((unused)) int   pid,
                         __attribute__ ((unused)) int   len,
                         __attribute__ ((unused)) int   arg2,
                         __attribute__ ((unused)) void* ud) {
  started = true;
}

static void log_done_callback(__attribute__ ((unused)) int   pid,
                              __attribute__ ((unused)) int   len,
                              __attribute__ ((unused)) int   arg2,
                              __attribute__ ((unused)) void* ud) {
  log_done = true;
}

static void wait_for_start(void) {
  // Register an IPC callback and wait for it to be called by the
  // screen app based on the user's app selection.
  ipc_register_service_callback("org.tockos.tutorials.attestation.questionable", ipc_callback,
                                NULL);
  yield_for(&started);
}

static int setup_logging() {
  returncode_t ret;

  // Find the PID of the screen logging service
  ret = ipc_discover(SCREEN_SERVICE_NAME, &screen_service);
  if (ret != RETURNCODE_SUCCESS) {
    printf("Screen logging service not found.\n");
    return ret;
  }

  // Set up a callback and share so we can supply log messages
  // and know when they've been completely logged.
  ipc_register_client_callback(screen_service, log_done_callback, NULL);
  ipc_share(screen_service, log_buf, LOG_WIDTH);

  return 0;
}

static int log_to_screen(const char* message) {
  returncode_t ret;

  // Load the log buffer with our message
  strlcpy(log_buf, message, LOG_WIDTH);

  // Start the logging process.
  ret = ipc_notify_service(screen_service);
  if (ret != RETURNCODE_SUCCESS) {
    printf("Failed to request a log to screen.\n");
    return ret;
  }

  // Wait for the log to complete.
  yield_for(&log_done);
  log_done = false;

  return 0;
}

int main(void) {
  returncode_t ret;

  // Wait to receive the signal to start from the app selector.
  wait_for_start();

  // Set up logging service.
  ret = setup_logging();
  if (ret < 0) {
    printf("ERROR: cannot set up logging\n");
  }

  // Declare our intention to hardfault everything.
  ret = log_to_screen("Faulting all applications...\n");
  if (ret < 0) {
    printf("ERROR: cannot log to screen\n");
  }

  // Bring everything down!
  syscall_return_t cr = command(0x99999, 2, 0, 0);
  if (cr.type != TOCK_SYSCALL_SUCCESS) {
    return tock_command_return_novalue_to_returncode(cr);
  }

  // Note if the application is still running.
  ret = log_to_screen("This should never be seen!");
  if (ret < 0) {
    printf("ERROR: cannot log to screen\n");
  }

  return 0;
}
