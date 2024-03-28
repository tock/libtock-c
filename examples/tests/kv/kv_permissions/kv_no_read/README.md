KV Permission Unit Test - No Read Permissions
================

This test app has no read permissions.

Needs to have the `examples/services/unit_test_supervisor` app installed as
well.

Output
------

Should get expected output like:

```
3.000: exists                   [✓]
3.001: set_get_no_permissions   [✓]
Summary 3: [2/2] Passed, [0/2] Failed, [0/2] Incomplete
```
