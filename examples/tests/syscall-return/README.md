Syscall Return Test
===================

Try every syscall type and return variant and ensure the return values match
what is expected.

Expected Output
---------------

```
syscall-return test
cmd 0: success()
  SUCCESS
cmd 1: failure(FAIL)
  SUCCESS
cmd 2: failure_u32(BUSY, 0x20000001)
  SUCCESS
cmd 3: failure_u32_u32(NOMEM, 0x30000001, 0x30000002)
  SUCCESS
cmd 4: failure_u64(INVAL, 0x4000000000000001)
  SUCCESS
cmd 5: success()
  SUCCESS
cmd 6: success_u32(0x60000001)
  SUCCESS
cmd 7: success_u32_u32(0x70000001, 0x70000002)
  SUCCESS
cmd 8: success_u64(0x8000000000000001)
  SUCCESS
cmd 9: success_u32_u32_u32(0x90000001, 0x90000002, 0x90000003)
  SUCCESS
cmd 10: success_u32_u64(0xA0000001, 0xA000000000000002)
  SUCCESS
allow_ro success (first call, expect prev ptr=0 len=0)
  SUCCESS
allow_ro success (second call, expect prev ptr=ro_buf len=16)
  SUCCESS
allow_ro failure (invalid ptr 0x90, expect INVAL ptr=0x90 size=4)
  SUCCESS
allow_ro failure (invalid ptr 0x9000000000000000, expect INVAL ptr=0x9000000000000000 size=4)
  SUCCESS
allow_rw success (first call, expect prev ptr=0 len=0)
  SUCCESS
allow_rw success (second call, expect prev ptr=rw_buf len=16)
  SUCCESS
allow_rw failure (invalid ptr 0x90, expect INVAL ptr=0x90 size=4)
  SUCCESS
allow_rw failure (invalid ptr 0xa000000000000000, expect INVAL ptr=0xa000000000000000 size=4)
  SUCCESS
allow_ur success (first call, expect prev ptr=0 len=0)
  SUCCESS
allow_ur success (second call, expect prev ptr=ur_buf len=16)
  SUCCESS
allow_ur failure (invalid ptr 0x90, expect INVAL ptr=0x90 size=4)
  SUCCESS
allow_ur failure (invalid ptr 0xb000000000000000, expect INVAL ptr=0xb000000000000000 size=4)
  SUCCESS
subscribe success (first call, expect prev cb=NULL data=NULL)
  SUCCESS
subscribe success (second call, expect prev cb=dummy_upcall data=NULL)
  SUCCESS
subscribe failure (invalid fn ptr 0x90, expect INVAL cb=0x90 data=0xfffffffffffffffe)
  SUCCESS
subscribe failure (invalid fn ptr 0xc000000000000000, expect INVAL cb=0xc000000000000000 data=0xfffffffffffffffe)
  SUCCESS
command: non-existent driver
  SUCCESS
allow_ro: non-existent driver
  SUCCESS
allow_rw: non-existent driver
  SUCCESS
allow_ur: non-existent driver
  SUCCESS
subscribe: non-existent driver
  SUCCESS
command: unknown command number 99 (expect NOSUPPORT)
  SUCCESS
subscribe: unknown subscribe number 9 (expect NOSUPPORT cb=dummy_upcall data=0x1234)
  SUCCESS
allow_ro: unknown allow number 9 (expect NOSUPPORT ptr=ro_buf size=16)
  SUCCESS
allow_rw: unknown allow number 9 (expect NOSUPPORT ptr=rw_buf size=16)
  SUCCESS
allow_ur: unsupported which 9 (expect NOSUPPORT ptr=ur_buf size=16)
  SUCCESS
memop 2: app RAM start (expect success, nonzero)
  SUCCESS
memop 3: app RAM end (expect success, > RAM start)
  SUCCESS
memop 1: sbrk(0) current break (expect success, RAM start <= brk <= RAM end)
  SUCCESS
memop 0: brk(current break) no-op (expect success, no value)
  SUCCESS
memop 6: grant region start (expect success, brk <= grant start <= RAM end)
  SUCCESS
memop 4: app flash start (expect success, nonzero)
  SUCCESS
memop 5: app flash end (expect success, > flash start)
  SUCCESS
memop 7: number of writeable flash regions (expect success)
  SUCCESS
memop 8: writeable flash region start, bad index 0 (expect FAIL)
  SUCCESS
memop 9: writeable flash region end, bad index 0 (expect FAIL)
  SUCCESS
memop 0: brk(0) below app RAM (expect NOMEM)
  SUCCESS
memop 1: sbrk(0x70000000) past end of app RAM (expect NOMEM)
  SUCCESS
memop 1: sbrk(0) unchanged after failed brk/sbrk (expect brk=0x...)
  SUCCESS
memop 99: unknown operation (expect NOSUPPORT)
  SUCCESS
yield-no-wait: no upcall pending (expect return 0)
  SUCCESS
done
```