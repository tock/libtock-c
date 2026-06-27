#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <libtock/tock.h>

#define DRIVER_NUM 0xA0000

static void dummy_upcall(int   a __attribute__((unused)),
                         int   b __attribute__((unused)),
                         int   c __attribute__((unused)),
                         void* d __attribute__((unused))) {}

#define CHECK(test, fmt, ...)                             \
        do {                                              \
          if (test) {                                     \
            printf("  SUCCESS\n");                        \
          } else {                                        \
            printf("  FAILURE: " fmt "\n",##__VA_ARGS__); \
          }                                               \
        } while (0)

int main(void) {
  printf("syscall-return test\n");

  syscall_return_t ret;
  int rc;
  uint32_t v1, v2, v3;
  uint64_t v64;

  // 0: success()
  printf("cmd 0: success()\n");
  ret = command(DRIVER_NUM, 0, 0, 0);
  rc  = tock_command_return_novalue_to_returncode(ret);
  CHECK(rc == RETURNCODE_SUCCESS, "rc=%d", rc);

  // 1: failure(FAIL)
  printf("cmd 1: failure(FAIL)\n");
  ret = command(DRIVER_NUM, 1, 0, 0);
  rc  = tock_command_return_novalue_to_returncode(ret);
  CHECK(rc == RETURNCODE_FAIL, "rc=%d", rc);

  // 2: failure_u32(BUSY, 0x10000001)
  printf("cmd 2: failure_u32(BUSY, 0x10000001)\n");
  ret = command(DRIVER_NUM, 2, 0, 0);
  rc  = tock_command_return_failure_u32_to_returncode(ret, &v1);
  CHECK(rc == RETURNCODE_EBUSY && v1 == 0x10000001u,
        "rc=%d v1=0x%08" PRIx32, rc, v1);

  // 3: failure_u32_u32(NOMEM, 0x20000001, 0x20000002)
  printf("cmd 3: failure_u32_u32(NOMEM, 0x20000001, 0x20000002)\n");
  ret = command(DRIVER_NUM, 3, 0, 0);
  rc  = tock_command_return_failure_u32_u32_to_returncode(ret, &v1, &v2);
  CHECK(rc == RETURNCODE_ENOMEM && v1 == 0x20000001u && v2 == 0x20000002u,
        "rc=%d v1=0x%08" PRIx32 " v2=0x%08" PRIx32, rc, v1, v2);

  // 4: failure_u64(INVAL, 0x4000000000000001)
  printf("cmd 4: failure_u64(INVAL, 0x4000000000000001)\n");
  ret = command(DRIVER_NUM, 4, 0, 0);
  rc  = tock_command_return_failure_u64_to_returncode(ret, &v64);
  CHECK(rc == RETURNCODE_EINVAL && v64 == 0x4000000000000001ull,
        "rc=%d val=0x%016" PRIx64, rc, v64);

  // 5: success()
  printf("cmd 5: success()\n");
  ret = command(DRIVER_NUM, 5, 0, 0);
  rc  = tock_command_return_novalue_to_returncode(ret);
  CHECK(rc == RETURNCODE_SUCCESS, "rc=%d", rc);

  // 6: success_u32(0x60000001)
  printf("cmd 6: success_u32(0x60000001)\n");
  ret = command(DRIVER_NUM, 6, 0, 0);
  rc  = tock_command_return_u32_to_returncode(ret, &v1);
  CHECK(rc == RETURNCODE_SUCCESS && v1 == 0x60000001u,
        "rc=%d v1=0x%08" PRIx32, rc, v1);

  // 7: success_u32_u32(0x70000001, 0x70000002)
  printf("cmd 7: success_u32_u32(0x70000001, 0x70000002)\n");
  ret = command(DRIVER_NUM, 7, 0, 0);
  rc  = tock_command_return_u32_u32_to_returncode(ret, &v1, &v2);
  CHECK(rc == RETURNCODE_SUCCESS && v1 == 0x70000001u && v2 == 0x70000002u,
        "rc=%d v1=0x%08" PRIx32 " v2=0x%08" PRIx32, rc, v1, v2);

  // 8: success_u32_u32_u32(0x80000001, 0x80000002, 0x80000003)
  printf("cmd 8: success_u32_u32_u32(0x80000001, 0x80000002, 0x80000003)\n");
  ret = command(DRIVER_NUM, 8, 0, 0);
  rc  = tock_command_return_u32_u32_u32_to_returncode(ret, &v1, &v2, &v3);
  CHECK(rc == RETURNCODE_SUCCESS && v1 == 0x80000001u && v2 == 0x80000002u && v3 == 0x80000003u,
        "rc=%d v1=0x%08" PRIx32 " v2=0x%08" PRIx32 " v3=0x%08" PRIx32,
        rc, v1, v2, v3);

  // 9: success_u64(0x9000000000000001)
  printf("cmd 9: success_u64(0x9000000000000001)\n");
  ret = command(DRIVER_NUM, 9, 0, 0);
  rc  = tock_command_return_u64_to_returncode(ret, &v64);
  CHECK(rc == RETURNCODE_SUCCESS && v64 == 0x9000000000000001ull,
        "rc=%d val=0x%016" PRIx64, rc, v64);

  // 10: success_u32_u64(0xA0000001, 0xA000000000000002)
  printf("cmd 10: success_u32_u64(0xA0000001, 0xA000000000000002)\n");
  ret = command(DRIVER_NUM, 10, 0, 0);
  rc  = tock_command_return_u32_u64_to_returncode(ret, &v1, &v64);
  CHECK(rc == RETURNCODE_SUCCESS && v1 == 0xA0000001u && v64 == 0xA000000000000002ull,
        "rc=%d v1=0x%08" PRIx32 " val=0x%016" PRIx64,
        rc, v1, v64);

  // --- Allow read-only ---
  static uint8_t ro_buf[16];

  // allow_readonly success: first call returns previous (ptr=0, len=0)
  printf("allow_ro success (first call, expect prev ptr=0 len=0)\n");
  allow_ro_return_t aro = allow_readonly(DRIVER_NUM, 0, ro_buf, sizeof(ro_buf));
  rc = tock_allow_ro_return_to_returncode(aro);
  CHECK(rc == RETURNCODE_SUCCESS && aro.ptr == NULL && aro.size == 0,
        "rc=%d ptr=%p size=%zu", rc, aro.ptr, aro.size);

  // allow_readonly success: second call echoes back the buffer just registered
  printf("allow_ro success (second call, expect prev ptr=ro_buf len=16)\n");
  allow_ro_return_t aro2 = allow_readonly(DRIVER_NUM, 0, ro_buf, sizeof(ro_buf));
  rc = tock_allow_ro_return_to_returncode(aro2);
  CHECK(rc == RETURNCODE_SUCCESS && aro2.ptr == ro_buf && aro2.size == sizeof(ro_buf),
        "rc=%d ptr=%p size=%zu", rc, aro2.ptr, aro2.size);

  // allow_readonly failure: invalid pointer — kernel rejects and echoes ptr+size back
  printf("allow_ro failure (invalid ptr 0x90, expect INVAL ptr=0x90 size=4)\n");
  allow_ro_return_t aro_f = allow_readonly(DRIVER_NUM, 0, (void*)0x90, 4);
  rc = tock_allow_ro_return_to_returncode(aro_f);
  CHECK(rc == RETURNCODE_EINVAL && aro_f.ptr == (void*)0x90 && aro_f.size == 4,
        "rc=%d ptr=%p size=%zu", rc, aro_f.ptr, aro_f.size);

  // --- Allow read-write ---
  static uint8_t rw_buf[16];

  // allow_readwrite success: first call returns previous (ptr=0, len=0)
  printf("allow_rw success (first call, expect prev ptr=0 len=0)\n");
  allow_rw_return_t arw = allow_readwrite(DRIVER_NUM, 0, rw_buf, sizeof(rw_buf));
  rc = tock_allow_rw_return_to_returncode(arw);
  CHECK(rc == RETURNCODE_SUCCESS && arw.ptr == NULL && arw.size == 0,
        "rc=%d ptr=%p size=%zu", rc, arw.ptr, arw.size);

  // allow_readwrite success: second call echoes back the buffer just registered
  printf("allow_rw success (second call, expect prev ptr=rw_buf len=16)\n");
  allow_rw_return_t arw2 = allow_readwrite(DRIVER_NUM, 0, rw_buf, sizeof(rw_buf));
  rc = tock_allow_rw_return_to_returncode(arw2);
  CHECK(rc == RETURNCODE_SUCCESS && arw2.ptr == rw_buf && arw2.size == sizeof(rw_buf),
        "rc=%d ptr=%p size=%zu", rc, arw2.ptr, arw2.size);

  // allow_readwrite failure: invalid pointer — kernel rejects and echoes ptr+size back
  printf("allow_rw failure (invalid ptr 0x90, expect INVAL ptr=0x90 size=4)\n");
  allow_rw_return_t arw_f = allow_readwrite(DRIVER_NUM, 0, (void*)0x90, 4);
  rc = tock_allow_rw_return_to_returncode(arw_f);
  CHECK(rc == RETURNCODE_EINVAL && arw_f.ptr == (void*)0x90 && arw_f.size == 4,
        "rc=%d ptr=%p size=%zu", rc, arw_f.ptr, arw_f.size);

  // --- Allow userspace readable ---
  static uint8_t ur_buf[16];

  // allow_userspace_read success: first call returns previous (ptr=0, len=0)
  printf("allow_ur success (first call, expect prev ptr=0 len=0)\n");
  allow_userspace_r_return_t aur = allow_userspace_read(DRIVER_NUM, 0, ur_buf, sizeof(ur_buf));
  rc = tock_allow_userspace_r_return_to_returncode(aur);
  CHECK(rc == RETURNCODE_SUCCESS && aur.ptr == NULL && aur.size == 0,
        "rc=%d ptr=%p size=%zu", rc, aur.ptr, aur.size);

  // allow_userspace_read success: second call echoes back the buffer just registered
  printf("allow_ur success (second call, expect prev ptr=ur_buf len=16)\n");
  allow_userspace_r_return_t aur2 = allow_userspace_read(DRIVER_NUM, 0, ur_buf, sizeof(ur_buf));
  rc = tock_allow_userspace_r_return_to_returncode(aur2);
  CHECK(rc == RETURNCODE_SUCCESS && aur2.ptr == ur_buf && aur2.size == sizeof(ur_buf),
        "rc=%d ptr=%p size=%zu", rc, aur2.ptr, aur2.size);

  // allow_userspace_read failure: invalid pointer — kernel rejects and echoes ptr+size back
  printf("allow_ur failure (invalid ptr 0x90, expect INVAL ptr=0x90 size=4)\n");
  allow_userspace_r_return_t aur_f = allow_userspace_read(DRIVER_NUM, 0, (void*)0x90, 4);
  rc = tock_allow_userspace_r_return_to_returncode(aur_f);
  CHECK(rc == RETURNCODE_EINVAL && aur_f.ptr == (void*)0x90 && aur_f.size == 4,
        "rc=%d ptr=%p size=%zu", rc, aur_f.ptr, aur_f.size);

  // --- Subscribe ---

  // subscribe success: first call returns previous callback (NULL, NULL)
  printf("subscribe success (first call, expect prev cb=NULL data=NULL)\n");
  subscribe_return_t sub = subscribe(DRIVER_NUM, 0, dummy_upcall, NULL);
  rc = tock_subscribe_return_to_returncode(sub);
  CHECK(rc == RETURNCODE_SUCCESS && sub.callback == NULL && sub.userdata == NULL,
        "rc=%d cb=%p data=%p", rc, (void*)sub.callback, sub.userdata);

  // subscribe success: second call echoes back the previously registered callback
  printf("subscribe success (second call, expect prev cb=dummy_upcall data=NULL)\n");
  subscribe_return_t sub2 = subscribe(DRIVER_NUM, 0, dummy_upcall, NULL);
  rc = tock_subscribe_return_to_returncode(sub2);
  CHECK(rc == RETURNCODE_SUCCESS && sub2.callback == dummy_upcall && sub2.userdata == NULL,
        "rc=%d cb=%p data=%p", rc, (void*)sub2.callback, sub2.userdata);

  // subscribe failure: invalid function pointer — kernel rejects and echoes ptr+userdata back
  void* max_minus_one = (void*)(UINTPTR_MAX - 1);
  printf("subscribe failure (invalid fn ptr 0x90, expect INVAL cb=0x90 data=%p)\n", max_minus_one);
  subscribe_return_t sub_f = subscribe(DRIVER_NUM, 0, (subscribe_upcall*)0x90, max_minus_one);
  rc = tock_subscribe_return_to_returncode(sub_f);
  CHECK(rc == RETURNCODE_EINVAL &&
        sub_f.callback == (subscribe_upcall*)0x90 && sub_f.userdata == max_minus_one,
        "rc=%d cb=%p data=%p", rc, (void*)sub_f.callback, sub_f.userdata);

  printf("done\n");
  return 0;
}
