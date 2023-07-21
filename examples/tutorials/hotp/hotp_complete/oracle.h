#ifndef ORACLE_H_
#define ORACLE_H_

#include <stdint.h>

int decrypt(const uint8_t* iv, const uint8_t* cipher, int cipher_len, uint8_t* output, int outputlen);

int encrypt(const uint8_t* plaintext, int plaintext_len, uint8_t* output, int output_len, uint8_t iv[16]);

#endif // ORACLE_H_
