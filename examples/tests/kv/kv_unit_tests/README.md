KV Unit Test App
================

Runs a series of unit tests for the KV store.

Needs to have the `examples/services/unit_test_supervisor` app installed as
well.

Output
------

Should get expected output like:

```
1.000: exists                   [✓]
1.001: set_get                  [✓]
1.002: set_get_too_long         [✓]
1.003: key_too_long             [✓]
1.004: set_value_too_long       [✓]
1.005: get_not_found            [✓]
1.006: add                      [✓]
1.007: add_add                  [✓]
1.008: update                   [✓]
1.009: update_no_exist          [✓]
1.010: delete                   [✓]
1.011: delete_delete            [✓]
1.012: set_get_32regions_1      [✓]
1.013: set_get_32regions_2      [✓]
1.014: set_get_32regions_3      [✓]
1.015: set_get_32regions_4      [✓]
1.016: set_get_32regions_5      [✓]
1.017: set_get_32regions_6      [✓]
1.018: set_get_32regions_7      [✓]
1.019: set_get_32regions_8      [✓]
Summary 1: [20/20] Passed, [0/20] Failed, [0/20] Incomplete
```
