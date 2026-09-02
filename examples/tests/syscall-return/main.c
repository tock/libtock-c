#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <libtock/tock.h>

#define DRIVER_NUM 0xB0000
#define DRIVER_NUM_NON_EXISTENT 0xF9876101

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

  // 2: failure_u32(BUSY, 0x20000001)
  printf("cmd 2: failure_u32(BUSY, 0x20000001)\n");
  ret = command(DRIVER_NUM, 2, 0, 0);
  rc  = tock_command_return_failure_u32_to_returncode(ret, &v1);
  CHECK(rc == RETURNCODE_EBUSY && v1 == 0x20000001u,
        "rc=%d v1=0x%08" PRIx32, rc, v1);

  // 3: failure_u32_u32(NOMEM, 0x30000001, 0x30000002)
  printf("cmd 3: failure_u32_u32(NOMEM, 0x30000001, 0x30000002)\n");
  ret = command(DRIVER_NUM, 3, 0, 0);
  rc  = tock_command_return_failure_u32_u32_to_returncode(ret, &v1, &v2);
  CHECK(rc == RETURNCODE_ENOMEM && v1 == 0x30000001u && v2 == 0x30000002u,
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

  // 8: success_u64(0x8000000000000001)
  printf("cmd 8: success_u64(0x8000000000000001)\n");
  ret = command(DRIVER_NUM, 8, 0, 0);
  rc  = tock_command_return_u64_to_returncode(ret, &v64);
  CHECK(rc == RETURNCODE_SUCCESS && v64 == 0x8000000000000001ull,
        "rc=%d val=0x%016" PRIx64, rc, v64);

  // 9: success_u32_u32_u32(0x90000001, 0x90000002, 0x90000003)
  printf("cmd 9: success_u32_u32_u32(0x90000001, 0x90000002, 0x90000003)\n");
  ret = command(DRIVER_NUM, 9, 0, 0);
  rc  = tock_command_return_u32_u32_u32_to_returncode(ret, &v1, &v2, &v3);
  CHECK(rc == RETURNCODE_SUCCESS && v1 == 0x90000001u && v2 == 0x90000002u && v3 == 0x90000003u,
        "rc=%d v1=0x%08" PRIx32 " v2=0x%08" PRIx32 " v3=0x%08" PRIx32,
        rc, v1, v2, v3);

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

#if defined(__riscv) && __riscv_xlen == 64
  // allow_readonly failure: invalid pointer — kernel rejects and echoes ptr+size back
  printf("allow_ro failure (invalid ptr 0x9000000000000000, expect INVAL ptr=0x9000000000000000 size=4)\n");
  allow_ro_return_t aro_f2 = allow_readonly(DRIVER_NUM, 0, (void*)0x9000000000000000, 4);
  rc = tock_allow_ro_return_to_returncode(aro_f2);
  CHECK(rc == RETURNCODE_EINVAL && aro_f2.ptr == (void*)0x9000000000000000 && aro_f2.size == 4,
        "rc=%d ptr=%p size=%zu", rc, aro_f2.ptr, aro_f2.size);
#endif

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

#if defined(__riscv) && __riscv_xlen == 64
  // allow_readwrite failure: invalid pointer — kernel rejects and echoes ptr+size back
  printf("allow_rw failure (invalid ptr 0xa000000000000000, expect INVAL ptr=0xa000000000000000 size=4)\n");
  allow_rw_return_t arw_f2 = allow_readwrite(DRIVER_NUM, 0, (void*)0xa000000000000000, 4);
  rc = tock_allow_rw_return_to_returncode(arw_f2);
  CHECK(rc == RETURNCODE_EINVAL && arw_f2.ptr == (void*)0xa000000000000000 && arw_f2.size == 4,
        "rc=%d ptr=%p size=%zu", rc, arw_f2.ptr, arw_f2.size);
#endif

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

#if defined(__riscv) && __riscv_xlen == 64
  // allow_userspace_read failure: invalid pointer — kernel rejects and echoes ptr+size back
  printf("allow_ur failure (invalid ptr 0xb000000000000000, expect INVAL ptr=0xb000000000000000 size=4)\n");
  allow_userspace_r_return_t aur_f2 = allow_userspace_read(DRIVER_NUM, 0, (void*)0xb000000000000000, 4);
  rc = tock_allow_userspace_r_return_to_returncode(aur_f2);
  CHECK(rc == RETURNCODE_EINVAL && aur_f2.ptr == (void*)0xb000000000000000 && aur_f2.size == 4,
        "rc=%d ptr=%p size=%zu", rc, aur_f2.ptr, aur_f2.size);
#endif

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

#if defined(__riscv) && __riscv_xlen == 64
  // subscribe failure: invalid function pointer — kernel rejects and echoes ptr+userdata back
  printf("subscribe failure (invalid fn ptr 0xc000000000000000, expect INVAL cb=0xc000000000000000 data=%p)\n",
         max_minus_one);
  subscribe_return_t sub_f2 = subscribe(DRIVER_NUM, 0, (subscribe_upcall*)0xc000000000000000, max_minus_one);
  rc = tock_subscribe_return_to_returncode(sub_f2);
  CHECK(rc == RETURNCODE_EINVAL &&
        sub_f2.callback == (subscribe_upcall*)0xc000000000000000 && sub_f2.userdata == max_minus_one,
        "rc=%d cb=%p data=%p", rc, (void*)sub_f2.callback, sub_f2.userdata);
#endif

  // --- Non-existent drivers ---

  // command
  printf("command: non-existent driver\n");
  ret = command(DRIVER_NUM_NON_EXISTENT, 0, 0, 0);
  rc  = tock_command_return_novalue_to_returncode(ret);
  CHECK(rc == RETURNCODE_ENODEVICE, "rc=%d", rc);

  // allow_readonly
  printf("allow_ro: non-existent driver\n");
  aro = allow_readonly(DRIVER_NUM_NON_EXISTENT, 0, NULL, 0);
  rc = tock_allow_ro_return_to_returncode(aro);
  CHECK(rc == RETURNCODE_ENODEVICE && aro.ptr == NULL && aro.size == 0,
        "rc=%d ptr=%p size=%zu", rc, aro.ptr, aro.size);

  // allow_readwrite
  printf("allow_rw: non-existent driver\n");
  arw = allow_readwrite(DRIVER_NUM_NON_EXISTENT, 0, NULL, 0);
  rc = tock_allow_rw_return_to_returncode(arw);
  CHECK(rc == RETURNCODE_ENODEVICE && arw.ptr == NULL && arw.size == 0,
        "rc=%d ptr=%p size=%zu", rc, arw.ptr, arw.size);

  // allow_userspace_read
  printf("allow_ur: non-existent driver\n");
  aur = allow_userspace_read(DRIVER_NUM_NON_EXISTENT, 0, NULL, 0);
  rc = tock_allow_userspace_r_return_to_returncode(aur);
  CHECK(rc == RETURNCODE_ENODEVICE && aur.ptr == NULL && aur.size == 0,
        "rc=%d ptr=%p size=%zu", rc, aur.ptr, aur.size);

  // subscribe
  printf("subscribe: non-existent driver\n");
  sub_f = subscribe(DRIVER_NUM_NON_EXISTENT, 0, NULL, 0);
  rc = tock_subscribe_return_to_returncode(sub_f);
  CHECK(rc == RETURNCODE_ENODEVICE &&
        sub_f.callback == (subscribe_upcall*)NULL && sub_f.userdata == 0,
        "rc=%d cb=%p data=%p", rc, (void*)sub_f.callback, sub_f.userdata);

  // --- Unsupported subdriver numbers on an existing driver ---
  //
  // The capsule declares one upcall / one allow-ro / one allow-rw slot (num 0)
  // and only accepts allow-userspace-readable `which` 0. Any other number is
  // rejected by the kernel (subscribe / allow ro+rw) or the capsule
  // (userspace-readable) with NOSUPPORT, and the pointers/values are echoed
  // back unchanged.

  // command: unknown command number -> capsule returns failure(NOSUPPORT)
  printf("command: unknown command number 99 (expect NOSUPPORT)\n");
  ret = command(DRIVER_NUM, 99, 0, 0);
  rc  = tock_command_return_novalue_to_returncode(ret);
  CHECK(rc == RETURNCODE_ENOSUPPORT, "rc=%d", rc);

  // subscribe: unknown subscribe number -> kernel returns NOSUPPORT, echoes cb+data
  printf("subscribe: unknown subscribe number 9 (expect NOSUPPORT cb=dummy_upcall data=0x1234)\n");
  subscribe_return_t sub_bad = subscribe(DRIVER_NUM, 9, dummy_upcall, (void*)0x1234);
  rc = tock_subscribe_return_to_returncode(sub_bad);
  CHECK(rc == RETURNCODE_ENOSUPPORT &&
        sub_bad.callback == dummy_upcall && sub_bad.userdata == (void*)0x1234,
        "rc=%d cb=%p data=%p", rc, (void*)sub_bad.callback, sub_bad.userdata);

  // allow_readonly: unknown allow number -> kernel returns NOSUPPORT, echoes ptr+size
  printf("allow_ro: unknown allow number 9 (expect NOSUPPORT ptr=ro_buf size=16)\n");
  allow_ro_return_t aro_bad = allow_readonly(DRIVER_NUM, 9, ro_buf, sizeof(ro_buf));
  rc = tock_allow_ro_return_to_returncode(aro_bad);
  CHECK(rc == RETURNCODE_ENOSUPPORT && aro_bad.ptr == ro_buf && aro_bad.size == sizeof(ro_buf),
        "rc=%d ptr=%p size=%zu", rc, aro_bad.ptr, aro_bad.size);

  // allow_readwrite: unknown allow number -> kernel returns NOSUPPORT, echoes ptr+size
  printf("allow_rw: unknown allow number 9 (expect NOSUPPORT ptr=rw_buf size=16)\n");
  allow_rw_return_t arw_bad = allow_readwrite(DRIVER_NUM, 9, rw_buf, sizeof(rw_buf));
  rc = tock_allow_rw_return_to_returncode(arw_bad);
  CHECK(rc == RETURNCODE_ENOSUPPORT && arw_bad.ptr == rw_buf && arw_bad.size == sizeof(rw_buf),
        "rc=%d ptr=%p size=%zu", rc, arw_bad.ptr, arw_bad.size);

  // allow_userspace_read: unsupported `which` -> capsule returns NOSUPPORT, echoes ptr+size
  printf("allow_ur: unsupported which 9 (expect NOSUPPORT ptr=ur_buf size=16)\n");
  allow_userspace_r_return_t aur_bad = allow_userspace_read(DRIVER_NUM, 9, ur_buf, sizeof(ur_buf));
  rc = tock_allow_userspace_r_return_to_returncode(aur_bad);
  CHECK(rc == RETURNCODE_ENOSUPPORT && aur_bad.ptr == ur_buf && aur_bad.size == sizeof(ur_buf),
        "rc=%d ptr=%p size=%zu", rc, aur_bad.ptr, aur_bad.size);

  // --- Memop ---
  //
  // Memop returns are not driver-specific: exercise the well-defined operations
  // and check the return variant plus the invariants between the reported
  // addresses.

  memop_return_t m;
  uintptr_t ram_start, ram_end, cur_brk, grant_start, flash_start, flash_end;

  // memop 2: start of app RAM (SuccessPtr / SuccessU32)
  printf("memop 2: app RAM start (expect success, nonzero)\n");
  m = memop(2, 0);
  ram_start = m.data;
  CHECK(m.status == TOCK_STATUSCODE_SUCCESS && ram_start != 0,
        "status=%d data=0x%" PRIxPTR, m.status, m.data);

  // memop 3: end of app RAM (SuccessPtr / SuccessU32)
  printf("memop 3: app RAM end (expect success, > RAM start)\n");
  m = memop(3, 0);
  ram_end = m.data;
  CHECK(m.status == TOCK_STATUSCODE_SUCCESS && ram_end > ram_start,
        "status=%d data=0x%" PRIxPTR " ram_start=0x%" PRIxPTR, m.status, m.data, ram_start);

  // memop 1: sbrk(0) returns the current break, unchanged (SuccessPtr / SuccessU32)
  printf("memop 1: sbrk(0) current break (expect success, RAM start <= brk <= RAM end)\n");
  m = memop(1, 0);
  cur_brk = m.data;
  CHECK(m.status == TOCK_STATUSCODE_SUCCESS && cur_brk >= ram_start && cur_brk <= ram_end,
        "status=%d brk=0x%" PRIxPTR, m.status, m.data);

  // memop 0: brk to the current break is a no-op (Success, no value)
  printf("memop 0: brk(current break) no-op (expect success, no value)\n");
  m = memop(0, cur_brk);
  CHECK(m.status == TOCK_STATUSCODE_SUCCESS && m.data == 0,
        "status=%d data=0x%" PRIxPTR, m.status, m.data);

  // memop 6: start of the grant region (SuccessAddr / SuccessU32)
  printf("memop 6: grant region start (expect success, brk <= grant start <= RAM end)\n");
  m = memop(6, 0);
  grant_start = m.data;
  CHECK(m.status == TOCK_STATUSCODE_SUCCESS && grant_start >= cur_brk && grant_start <= ram_end,
        "status=%d grant_start=0x%" PRIxPTR, m.status, m.data);

  // memop 4: start of app flash (SuccessPtr / SuccessU32)
  printf("memop 4: app flash start (expect success, nonzero)\n");
  m = memop(4, 0);
  flash_start = m.data;
  CHECK(m.status == TOCK_STATUSCODE_SUCCESS && flash_start != 0,
        "status=%d data=0x%" PRIxPTR, m.status, m.data);

  // memop 5: end of app flash (SuccessPtr / SuccessU32)
  printf("memop 5: app flash end (expect success, > flash start)\n");
  m = memop(5, 0);
  flash_end = m.data;
  CHECK(m.status == TOCK_STATUSCODE_SUCCESS && flash_end > flash_start,
        "status=%d data=0x%" PRIxPTR " flash_start=0x%" PRIxPTR, m.status, m.data, flash_start);

  // memop 7: number of writeable flash regions (SuccessU32)
  printf("memop 7: number of writeable flash regions (expect success)\n");
  m = memop(7, 0);
  CHECK(m.status == TOCK_STATUSCODE_SUCCESS,
        "status=%d data=0x%" PRIxPTR, m.status, m.data);

  // memop with an unknown operation -> Failure(NOSUPPORT)
  printf("memop 99: unknown operation (expect NOSUPPORT)\n");
  m = memop(99, 0);
  CHECK(m.status == TOCK_STATUSCODE_NOSUPPORT,
        "status=%d", m.status);

  // --- Yield-no-wait ---
  //
  // All upcalls from earlier syscalls (e.g. the synchronous console writes for
  // these printfs) have already been serviced, so no upcall is pending.
  printf("yield-no-wait: no upcall pending (expect return 0)\n");
  int yield_ret = yield_no_wait();
  CHECK(yield_ret == 0, "yield_no_wait=%d", yield_ret);

  printf("done\n");
  return 0;
}
