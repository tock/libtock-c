#pragma once

#define YM_BLOCK_SIZE_SHORT 128
#define YM_BLOCK_SIZE_LONG  1024

// Longest filename (including NUL) this app will accept from a header
// block.
#define YM_MAX_FILENAME 64

void ymodem_start(uint8_t* block_data);