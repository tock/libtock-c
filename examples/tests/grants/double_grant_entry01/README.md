Double Grant Entry Test
=======================

This verifies the kernel will not allow a capsule to enter a grant twice.

Usage
-----

You need to add the `capsules/src/test/double_grant_entry.rs` to the board's
syscall interface.

Here is an example:

```diff
diff --git a/boards/hail/src/main.rs b/boards/hail/src/main.rs
index 110d45fa7..e8f4728c2 100644
--- a/boards/hail/src/main.rs
+++ b/boards/hail/src/main.rs
@@ -73,6 +73,7 @@ struct Hail {
     ipc: kernel::ipc::IPC<NUM_PROCS>,
     crc: &'static capsules::crc::Crc<'static, sam4l::crccu::Crccu<'static>>,
     dac: &'static capsules::dac::Dac<'static>,
+    dge: &'static capsules::test::double_grant_entry::TestGrantDoubleEntry,
 }

 /// Mapping of integer syscalls to objects that implement syscalls.
@@ -102,6 +103,8 @@ impl Platform for Hail {

             capsules::dac::DRIVER_NUM => f(Some(self.dac)),

+            capsules::test::double_grant_entry::DRIVER_NUM => f(Some(self.dge)),
+
             kernel::ipc::DRIVER_NUM => f(Some(&self.ipc)),
             _ => f(None),
         }
@@ -396,6 +399,14 @@ pub unsafe fn reset_handler() {
         capsules::dac::Dac::new(&peripherals.dac)
     );

+    // Test double grant entry
+    let dge = static_init!(
+        capsules::test::double_grant_entry::TestGrantDoubleEntry,
+        capsules::test::double_grant_entry::TestGrantDoubleEntry::new(
+            board_kernel.create_grant(&memory_allocation_capability)
+        )
+    );
+
     // // DEBUG Restart All Apps
     // //
     // // Uncomment to enable a button press to restart all apps.
@@ -440,6 +451,7 @@ pub unsafe fn reset_handler() {
         ipc: kernel::ipc::IPC::new(board_kernel, &memory_allocation_capability),
         crc,
         dac,
+        dge,
     };

     // Setup the UART bus for nRF51 serialization..
```



Expected Output
---------------

A panic with message "Attempted to re-enter a grant region." indicates success
and that things are working as expected.

```
Initialization complete. Entering main loop.
[TEST] Double Grant Entry 1
This test uses a capsule to verify a grant cannot be accessed
twice simultaneously. When this runs you will likely see a panic
which means the double grant enter was prevented and this test
was successful. If you see an "ERROR" message then this failed.


panicked at 'Attempted to re-enter a grant region.', kernel/src/grant.rs:340:17
	Kernel version release-1.6-1173-g9a49a9349

---| No debug queue found. You can set it with the DebugQueue component.

---| Cortex-M Fault Status |---
No Cortex-M faults detected.

...panic continues...
```