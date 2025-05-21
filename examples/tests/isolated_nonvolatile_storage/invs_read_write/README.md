Isolated Nonvolatile Storage Test App
=====================================

This app writes to flash storage and reads it back to test that isolated storage
is working.

Example Output
--------------

```
[TEST] Isolated Nonvolatile Storage
Have 4096 bytes of nonvolatile storage
     Test with read size 14 and write size 256 ...
     Test with read size 14 and write size 256 ...
     Test with read size 512 and write size 512 ...
Write to end of region (offset 3584)
     Test with read size 512 and write size 500 ...
Write beyond end region, should fail (offset 4096)
     Test with read size 512 and write size 501 ...
     ERROR calling write. returncode: -6
Write starts beyond end region, should fail (offset 4097)
     Test with read size 512 and write size 1 ...
     ERROR calling write. returncode: -6
Write starts before start region, should fail (offset -1)
     Test with read size 512 and write size 512 ...
     ERROR calling write. returncode: -6
All tests succeeded
```
