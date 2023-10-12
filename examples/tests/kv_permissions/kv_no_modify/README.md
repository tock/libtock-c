KV Permission Unit Test - No Modify Permissions
================

This test app has no modify permissions.

Needs to have the `examples/services/unit_test_supervisor` app installed as
well.

Output
------

Should get expected output like:

```
4.000: exists                   [✓]
4.001: set_get                  [✓]
4.002: set_set                  [✓]
4.003: set_delete               [✓]
Summary 4: [4/4] Passed, [0/4] Failed, [0/4] Incomplete
```
