# Hardware CI Tests

## Run Python Test

To run the test,
```bash
sudo python3 test.py Nrf52840GpioTest
```

Switch board name to the test you intend to run. Otherwise, python unittest
will attempt to run all tests.

Board | Test Name
------|----------
nrf52840dk | Nrf52840Test
