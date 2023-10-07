# Hardware CI Tests

## Run Python Test

`test.py` provides basic test that send character over uart connection, and same expects character to be returned. Provides this test over a course of 45 seconds. If character returned was not the same in this buffer period, test will fail. Otherwise, correct character was returned and test will pass. 

To run the test,
```bash
sudo python3 test.py Nrf52840Test
```

Switch board name to the test you intend to run. Otherwise, python unittest
will attempt to run all tests.

Board | Test Name
------|----------
nrf52840dk | Nrf52840Test