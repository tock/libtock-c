KV Permission Unit Test - No Write Permission
================

This app has no write ID and cannot create KV objects.

Needs to have the `examples/services/unit_test_supervisor` app installed as
well.

Output
------

Should get expected output like:

```
2.000: exists                   [✓]
2.001: set_no_permissions       [✓]
Summary 2: [2/2] Passed, [0/2] Failed, [0/2] Incomplete
```
