#include <stdio.h>

#include <libtock-sync/storage/app_state.h>
#include <libtock/tock.h>

#define MAGIC 0xcafe

struct demo_app_state_t {
  uint32_t magic;
  uint32_t count;
};

// This macro sets up all of the state required to create a persistent data
// structure. It will automatically be registered with the library and setup
// a structure as if this line of code was here instead:
//
//   struct demo_app_state_t app_state;
//
// To get the initial structure from flash the app should call
// `app_state_load_sync()`. It can the write the struct like any normal
// variable. When the app wishes to "checkpoint" the state and write it back to
// the persistent flash, it should call `app_state_save_sync()`.
LIBTOCK_APP_STATE_DECLARE(struct demo_app_state_t, app_state);

int main(void) {
  int ret;

  ret = libtock_app_state_load();
  if (ret < 0) {
    printf("Error loading application state: %s\n", tock_strrcode(ret));
    return ret;
  }

  if (app_state.magic != MAGIC) {
    printf("Application has never saved state before\n");
    app_state.magic = MAGIC;
    app_state.count = 1;
  } else {
    char plural[2] = {'s', 0};
    if (app_state.count == 1) {
      *plural = 0;
    }
    printf("This application has run %lu time%s before\n", app_state.count, plural);
    app_state.count += 1;
  }

  ret = libtocksync_app_state_save();
  if (ret != 0) {
    printf("ERROR saving application state: %s\n", tock_strrcode(ret));
    return ret;
  }
  printf("State saved successfully. Done.\n");

  return 0;
}
