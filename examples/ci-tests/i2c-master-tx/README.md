# Hardware CI Tests

## Run Python Test

`test.py` provides basic test that sends message from master to slave, and waits for a 12 second period of master sending messages, but captures the first message sent. The message sent from master will be printed over a 12 second period. If message sent from master is the same as the expected message to be sent, then test will pass.

To run the test,
```bash
sudo python3 test.py Nrf52840Test
```

Switch board name to the test you intend to run. Otherwise, python unittest
will attempt to run all tests.

Board | Test Name
------|----------
nrf52840dk | Nrf52840Test

**NOTE**
This test requires the Raspberry Pi to be set as slave. Thus, you must use Broadcom Pins 10 & 11 (SDA & SCL resepectively) or GPIO pin 19 and 23. You must also download the open source python library "pigpio" which enables slave access on the Raspberry Pi. How to download the library is done here on **CI Hardware Documentation - Raspberry Pi setup**

This test requires button activation on tested boards. Two jumpers are required for the activation of the reset button and user button on boards. Broadcom Pins on Raspberry PI used are pins 20 & 21 (GPIO Pins 38 & 40) which are used for user button and reset respectively. The user button and reset button depend on board tested.