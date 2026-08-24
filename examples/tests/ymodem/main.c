// Licensed under the Apache License, Version 2.0 or the MIT License.
// SPDX-License-Identifier: Apache-2.0 OR MIT
// Copyright Tock Contributors 2026.

// YMODEM file receiver test app.
//
// Receives one or more files sent by a standard YMODEM sender (e.g.
// `sb`/`sx` from the `lrzsz` package) over the console/UART, using CRC-16
// mode. This app doesn't do anything with the received bytes other than
// verify them against the sender-declared file size and print a running
// checksum, to confirm the transfer worked -- it's a protocol test, not an
// app installer.
//
// Usage: start this app, then run (on the host, connected to the board's
// serial port):
//
//     sb <file>
//
// (piped to/from the serial device, or run inside a terminal program like
// `cu`/`minicom` that supports invoking `sb`/`sz` directly).
//
// Protocol notes / simplifications:
//
// - Only CRC-16 mode is implemented (this app always requests it by sending
//   'C'); the legacy single-byte checksum mode is not supported.
// - Both 128-byte (SOH) and 1024-byte (STX) blocks are accepted.
// - This app has no way to time out a `libtocksync_console_read()` call, so
//   there's no retry loop around the initial handshake: it sends a single
//   'C' and then blocks waiting for the sender's first block. Start the
//   host-side sender before or shortly after starting this app -- YMODEM
//   senders retry sending their first block for a while (`sb` waits about a
//   minute) waiting for exactly this 'C', so exact timing doesn't matter,
//   just don't wait too long between the two.
// - Once a transfer is underway, retries are safe with blocking reads: a
//   NAK causes the sender to resend the same block, so this app can just
//   block waiting for that resend.

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libtock-sync/interface/console.h>

#include "serial.h"

// YMODEM/XMODEM control bytes.
#define YM_SOH 0x01
#define YM_STX 0x02
#define YM_EOT 0x04
#define YM_ACK 0x06
#define YM_NAK 0x15
#define YM_CAN 0x18
#define YM_CRC_MODE_CHAR 'C'

#define YM_BLOCK_SIZE_SHORT 128
#define YM_BLOCK_SIZE_LONG  1024

// Longest filename (including NUL) this app will accept from a header
// block.
#define YM_MAX_FILENAME 64

// Scratch space for one data block. Static (not stack-allocated) since
// 1024 bytes may not comfortably fit alongside other locals on a small
// app stack.
static uint8_t block_data[YM_BLOCK_SIZE_LONG];

typedef enum {
  YM_BLOCK_OK,
  YM_BLOCK_EOT,
  YM_BLOCK_CANCEL,
  YM_BLOCK_ERROR,
} ym_block_result_t;

// Reads exactly `len` bytes into `buf`. Returns false on any console error
// or short read.
static bool ym_read_exact(uint8_t* buf, uint32_t len) {
  uint32_t read_count;
  uint32_t timeout = 500;
  // if (len > 2) {
  //   printf("do read len:%i\n", len);
  //   timeout=5000;
  // }else{
  //   printf("do read\n");
  // }
  returncode_t ret = libtocksync_ymodem_read_with_timeout(buf, len, &read_count, timeout);

  // if (len>2) {
  //   printf("read done %i, %i\n", ret, read_count);
  //   printf("got: ");
  //   for (int i=0;i<read_count;i++) {
  //     printf("%c", buf[i]);
  //   }
  //   printf("\n");
  // }else{
  //   printf("read done\n");
  // }
  return ret == RETURNCODE_SUCCESS && read_count == len;
}

static void ym_write_byte(uint8_t b) {
  uint32_t written;
  libtocksync_ymodem_write(&b, 1, &written);
}

// CRC-16/XMODEM: poly 0x1021, initial value 0, no reflection, no final XOR.
// This is the CRC variant YMODEM's "CRC mode" uses.
static uint16_t ym_crc16(const uint8_t* data, uint32_t len) {
  uint16_t crc = 0;
  for (uint32_t i = 0; i < len; i++) {
    crc ^= (uint16_t) data[i] << 8;
    for (int bit = 0; bit < 8; bit++) {
      if (crc & 0x8000) {
        crc = (uint16_t) ((crc << 1) ^ 0x1021);
      } else {
        crc = (uint16_t) (crc << 1);
      }
    }
  }
  return crc;
}

// Attempts to receive a single framed block (SOH/STX + block# + ~block# +
// data + CRC-16), or recognize an EOT/CAN control byte. On `YM_BLOCK_OK`,
// fills in `block_data` (module-global scratch buffer), `*data_len` (128 or
// 1024), and `*block_num`.
static ym_block_result_t ym_receive_block(uint32_t* data_len, uint8_t* block_num) {
  uint8_t marker;
  if (!ym_read_exact(&marker, 1)) return YM_BLOCK_ERROR;

  // printf("got byte %i\n", marker);

  if (marker == YM_EOT) return YM_BLOCK_EOT;
  if (marker == YM_CAN) return YM_BLOCK_CANCEL;

  uint32_t len;
  if (marker == YM_SOH) {
    len = YM_BLOCK_SIZE_SHORT;
  } else if (marker == YM_STX) {
    len = YM_BLOCK_SIZE_LONG;
  } else {
    // Unrecognized/garbage byte.
    return YM_BLOCK_ERROR;
  }

  uint8_t header[2];
  if (!ym_read_exact(header, 2)) return YM_BLOCK_ERROR;
  // printf("got header 2\n");
  if (!ym_read_exact(block_data, len)) return YM_BLOCK_ERROR;
  uint8_t crc_bytes[2];
  if (!ym_read_exact(crc_bytes, 2)) return YM_BLOCK_ERROR;

  // Block number and its ones'-complement must add to 0xFF.
  if ((uint8_t) (header[0] + header[1]) != 0xFF) return YM_BLOCK_ERROR;

  uint16_t received_crc = ((uint16_t) crc_bytes[0] << 8) | crc_bytes[1];
  if (ym_crc16(block_data, len) != received_crc) return YM_BLOCK_ERROR;

  *block_num = header[0];
  *data_len  = len;
  return YM_BLOCK_OK;
}

// Sends the CRC-mode handshake byte and receives/validates a YMODEM header
// (block 0). Returns true and fills `filename`/`filesize` if a file
// transfer is starting; returns false if the sender signaled end-of-batch
// (an empty-filename header) or cancelled.
static bool ym_receive_header(char* filename, uint32_t filename_len, uint32_t* filesize) {
  

  while (true) {
    uint32_t data_len;
    uint8_t block_num;

  //   printf("ymodem: waiting for sender (start e.g. `sb <file>` now)...\n");
  ym_write_byte(YM_CRC_MODE_CHAR);

    ym_block_result_t result = ym_receive_block(&data_len, &block_num);
// printf("received block or timeout\n");

    if (result == YM_BLOCK_ERROR) {
      // timeout
      continue;
    }

    // printf("received good news\n");

    if (result == YM_BLOCK_CANCEL) {
      printf("ymodem: transfer cancelled by sender\n");
      return false;
    }
    if (result != YM_BLOCK_OK || block_num != 0) {
      // Bad block, or an unexpected EOT/wrong block number for a header:
      // ask the sender to resend.
      ym_write_byte(YM_NAK);
      continue;
    }

    ym_write_byte(YM_ACK);

    if (block_data[0] == '\0') {
      // Empty filename marks the end of the batch.
      return false;
    }

    strncpy(filename, (char*) block_data, filename_len - 1);
    filename[filename_len - 1] = '\0';

    // The size (decimal ASCII) immediately follows the filename's NUL
    // terminator. Any further fields (mtime, mode, serial number) are
    // space-separated after it; atol() stops at the first non-digit, so
    // their presence doesn't matter here.
    const char* size_str = (const char*) block_data + strlen((char*) block_data) + 1;
    *filesize = (uint32_t) atol(size_str);

    return true;
  }
}

// Receives the data blocks of one file (after its header has been ACKed),
// until EOT. Prints progress and a final checksum; doesn't store the file
// anywhere.
static bool ym_receive_file(uint32_t filesize) {
  // Kick off the data phase.
  ym_write_byte(YM_CRC_MODE_CHAR);

  uint8_t expected_block  = 1;
  uint32_t total_received = 0;
  uint32_t checksum       = 0;

  while (true) {
    uint32_t data_len;
    uint8_t block_num;
    ym_block_result_t result = ym_receive_block(&data_len, &block_num);

    if (result == YM_BLOCK_EOT) {
      ym_write_byte(YM_ACK);
      break;
    }
    if (result == YM_BLOCK_CANCEL) {
      printf("ymodem: transfer cancelled by sender\n");
      return false;
    }
    if (result != YM_BLOCK_OK) {
      ym_write_byte(YM_NAK);
      continue;
    }

    if (block_num == (uint8_t) (expected_block - 1)) {
      // Sender resent the last block, likely because our ACK was lost.
      // Don't reprocess it, just ACK again.
      ym_write_byte(YM_ACK);
      continue;
    }
    if (block_num != expected_block) {
      ym_write_byte(YM_NAK);
      continue;
    }

    // The final block is padded with 0x1A up to the full block size; only
    // count bytes up through the sender-declared file size as real data.
    uint32_t copy_len = data_len;
    if (filesize > 0) {
      uint32_t remaining = (total_received < filesize) ? (filesize - total_received) : 0;
      copy_len = (remaining < data_len) ? remaining : data_len;
    }

    // printf("got: ");
    // for (int i=0;i<copy_len;i++) {
    //   printf("%c", block_data[i]);
    // }
    // printf("\n");


    for (uint32_t i = 0; i < copy_len; i++) {
      checksum += block_data[i];
    }
    total_received += copy_len;

    expected_block++; // Wraps at 256, matching the protocol's block numbering.
    ym_write_byte(YM_ACK);

    printf("ymodem: received %i bytes\n", (int) total_received);
  }
  printf("file loop done\n");

  printf("ymodem: done, %i bytes received (expected %i), checksum=0x%08i\n",
         (int) total_received, (int) filesize, (int) checksum);

  printf("ymodem: done, %i bytes received (expected %i)\n",
         (int) total_received, (int) filesize);
  if (filesize > 0 && total_received != filesize) {
    printf("ymodem: WARNING size mismatch\n");
  }

  return true;
}

int main(void) {
  printf("YMODEM receiver test\n");

  printf("ymodem: waiting for sender (start e.g. `sb <file>` now)...\n");
  // uint32_t data_len;
  // uint8_t block_num;

  // while (true) {
  //   ym_write_byte(YM_CRC_MODE_CHAR);

  //   ym_block_result_t result = ym_receive_block(&data_len, &block_num);

  //   if (result == YM_BLOCK_ERROR) {
  //     // timeout
  //     continue;
  //   } else {
  //     break;
  //   }
  // }


  while (true) {
    char filename[YM_MAX_FILENAME];
    uint32_t filesize;

    if (!ym_receive_header(filename, sizeof(filename), &filesize)) {
      printf("ymodem: batch complete\n");
      break;
    }

    printf("ymodem: receiving \"%s\" (%lu bytes)\n", filename, (unsigned long) filesize);

    if (!ym_receive_file(filesize)) {
      printf("ymodem: transfer of \"%s\" failed\n", filename);
      break;
    }
  }

  return 0;
}
