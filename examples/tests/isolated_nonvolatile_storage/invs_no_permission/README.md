Isolated Nonvolatile Storage Test App With No Permissions
=========================================================

This app tries to use the isolated nonvolatile storage capsule but is granted no
storage access permissions (its `write_id` in the TBF header is 0). This
verifies that the read and write calls return `ENOSUPPORT`.
