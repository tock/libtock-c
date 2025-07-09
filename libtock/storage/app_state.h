#pragma once

// Utility for allowing apps to read and write a region of flash. This interface
// is designed for apps to specify a special region of flash that stores
// persistent data, and not as a general purpose flash reading and writing tool.
//
// The expected use looks something like:
//
//   struct valuable_data {
//     uint32_t magic;
//     uint32_t my_value1;
//     uint32_t my_value2;
//   }
//
//   // Setup the struct that has the app state.
//   APP_STATE_DECLARE(struct valuable_data, my_data);
//
//   int main() {
//     int ret;
//
//     // Get the initial copy of the data in flash.
//     ret = app_state_load_sync();
//     if (ret != 0) prinrf("ERROR(%i): Could not read the flash region.\n", ret);
//
//     // Check that the magic value is as expected.
//     if (data_in_ram.magic != MAGIC) {
//       // do some initialization
//     }
//
//     // do other computation
//
//     // Save changed valuable data.
//     ret = app_state_save_sync();
//     if (ret != 0) prinrf("ERROR(%i): Could not write back to flash.\n", ret);
//   }

#include "../tock.h"
#include "syscalls/app_state_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Declare an application state structure
//
// This macro does a little extra bookkeeping, however it should look
// much like a simple declaration in practice:
//
//     APP_STATE_DECLARE(struct my_state_t, memory_copy);
//
// Which you can read as the equivalent of:
//
//     struct my_state_t memory_copy;
//
// The variable `memory_copy` is available as regular C structure, however
// users must explicitly `load` and `save` application state as appropriate.
// Note that each process may only use APP_STATE_DECLARE once.
#define LIBTOCK_APP_STATE_DECLARE(_type, _identifier)                 \
        __attribute__((section(".app_state")))                              \
        _type _app_state_flash;                                             \
        _type _identifier;                                                  \
        void* _app_state_flash_pointer = NULL;                              \
        void* _app_state_ram_pointer   = &_identifier;                        \
        size_t _app_state_size         = sizeof(_type);                             \
        bool _app_state_inited         = false;


// Function signature save done callbacks.
//
// - `arg1` (`returncode_t`): Status of save operation.
typedef void (*libtock_app_state_callback)(returncode_t);


extern void* _app_state_flash_pointer;
extern void* _app_state_ram_pointer;
extern size_t _app_state_size;
extern bool _app_state_inited;



// Load application state from persistent storage into the in-memory storage
// location.
returncode_t libtock_app_state_load(void);

// Save the application state to persistent storage.
returncode_t libtock_app_state_save(libtock_app_state_callback cb);



#ifdef __cplusplus
}
#endif
