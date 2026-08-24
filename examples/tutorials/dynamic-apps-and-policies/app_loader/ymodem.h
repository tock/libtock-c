#pragma once

#define YM_BLOCK_SIZE_SHORT 128
#define YM_BLOCK_SIZE_LONG  1024

// Longest filename (including NUL) this app will accept from a header
// block.
#define YM_MAX_FILENAME 64

typedef void (*ymodem_cb_file_started)(uint32_t);
typedef void (*ymodem_cb_block_received)(uint8_t*, uint32_t);
typedef void (*ymodem_cb_file_received)(void);


void ymodem_start(uint8_t* block_data,
	ymodem_cb_file_started cb_file_started,
	ymodem_cb_block_received cb_block_received,
	ymodem_cb_file_received cb_file_received);