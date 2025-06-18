#include <stdbool.h>

#include <libtock-sync/peripherals/rng.h>
#include <stdio.h>
#include "oracle.h"

struct crypt_upcall_ud {
  bool done;
  int len;
};

static void crypt_upcall(__attribute__((unused)) int  num,
                         int                          len,
                         __attribute__ ((unused)) int arg2,
                         void*                        ud) {
  ((struct crypt_upcall_ud*)ud)->done = true;
  ((struct crypt_upcall_ud*)ud)->len  = len;
}

int oracle_decrypt(const uint8_t* iv, const uint8_t* cipher, int cipher_len, uint8_t* output, int outputlen) {
  // set IV
  allow_ro_return_t ror = allow_readonly(0x99999, 0, iv, 16);
  if (!ror.success) {
    assert(0);
    return tock_allow_ro_return_to_returncode(ror);
  }

  // set plaintext
  ror = allow_readonly(0x99999, 1, cipher, cipher_len);
  if (!ror.success) {
    return tock_allow_ro_return_to_returncode(ror);
  }

  allow_rw_return_t rwr = allow_readwrite(0x99999, 0, output, outputlen);
  if (!rwr.success) {
    return tock_allow_rw_return_to_returncode(rwr);
  }

  struct crypt_upcall_ud ud;
  ud.done = false;
  ud.len  = 0;
  subscribe_return_t sr = subscribe(0x99999, 0, &crypt_upcall, (void*)&ud);
  if (!sr.success) {
    return tock_subscribe_return_to_returncode(sr);
  }

  syscall_return_t cr = command(0x99999, 1, 0, 0);
  if (cr.type != TOCK_SYSCALL_SUCCESS) {
    return tock_command_return_novalue_to_returncode(cr);
  }

  yield_for(&ud.done);

  return ud.len;
}

int oracle_encrypt(const uint8_t* plaintext, int plaintext_len, uint8_t* output, int output_len, uint8_t iv[16]) {
  return oracle_decrypt(iv, plaintext, plaintext_len, output, output_len);
}
