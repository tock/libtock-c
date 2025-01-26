#include "sdcard.h"

// Internal callback for creating synchronous functions
//
// callback_type - number indicating which type of callback occurred
// arg1, arg2 - meaning varies based on callback_type
// callback_args - user data passed into the set_callback function
//
// Possible callbacks
// 0: card_detection_changed, SD card was either installed or removed
//    arg1, arg2 - no meaning
// 1: init_done, initialization completed successfully
//    arg1 - block_size, block size of SD card in bytes
//    arg2 - size_in_kB, total size of SD card in kilobytes
// 2: read_done, read block completed successfully
//    arg1 - len, number of bytes read
// 3: write_done, write block completed successfully
//    arg1 - len, number of bytes written
// 4: error, an error occurred
//    arg1 - error, number representing the error that occurred
static void sdcard_upcall(int callback_type, int arg1, int arg2, void* opaque) {
  switch (callback_type) {
    case 0: {
      // card_detection_changed
      libtock_sdcard_callback_operations cb = (libtock_sdcard_callback_operations) opaque;
      cb(RETURNCODE_EUNINSTALLED);
      break;
    }

    case 1: {
      // init_done
      libtock_sdcard_callback_initialized cb = (libtock_sdcard_callback_initialized) opaque;
      cb(RETURNCODE_SUCCESS, arg1, arg2);
      break;
    }

    case 2: {
      // read_done
      libtock_sdcard_callback_operations cb = (libtock_sdcard_callback_operations) opaque;
      cb(RETURNCODE_SUCCESS);
      break;
    }

    case 3: {
      // write_done
      libtock_sdcard_callback_operations cb = (libtock_sdcard_callback_operations) opaque;
      cb(RETURNCODE_SUCCESS);
      break;
    }

    case 4: {
      // error
      // This is not a STATUSCODE, but rather an SD Card specific error
      // See capsule sdcard.rs `enum SdCardError`
      libtock_sdcard_callback_operations cb = (libtock_sdcard_callback_operations) opaque;
      cb(arg1);
      break;
    }
  }
}

returncode_t libtock_sdcard_initialize(libtock_sdcard_callback_initialized cb) {
  returncode_t ret;

  ret = libtock_sdcard_set_upcall(sdcard_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_sdcard_command_initialize();
  return ret;
}

returncode_t libtock_sdcard_read_block(uint32_t sector, uint8_t* buffer, uint32_t len,
                                       libtock_sdcard_callback_operations cb) {
  returncode_t ret;

  ret = libtock_sdcard_set_upcall(sdcard_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_sdcard_set_readwrite_allow_read_buffer(buffer, len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_sdcard_command_read_block(sector);
  return ret;
}

returncode_t libtock_sdcard_write_block(uint32_t sector, uint8_t* buffer, uint32_t len,
                                        libtock_sdcard_callback_operations cb) {
  returncode_t ret;

  ret = libtock_sdcard_set_upcall(sdcard_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_sdcard_set_readonly_allow_write_buffer(buffer, len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_sdcard_command_write_block(sector);
  return ret;
}
