// driver for the encryption oracle capsule

#pragma once

#include <stdint.h>

int oracle_decrypt(const uint8_t* iv, const uint8_t* cipher, int cipher_len, uint8_t* output, int outputlen);