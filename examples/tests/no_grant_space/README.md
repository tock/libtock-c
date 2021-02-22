No Grant Space
==============

This app increases its `app_brk` as far as it can and then yields. This should
minimize grant space. However, due to MPU limitations, there may still be
process-unaccessible memory below the kernel_brk for that process.

