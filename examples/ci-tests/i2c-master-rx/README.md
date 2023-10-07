# Hardware CI Tests

## Run Python Test

`test.py` provides basic test that sends message from slave to master, and waits for master to send message containing the slave message. This is essentially a Rx/Tx test, but we are primarily focusing on the reception of master here. The message sent from master will be printed over a 12 second period. If message sent from master is the exact same from message sent from slave, the test will pass.

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

This test requires button activation on tested boards. Two jumpers are required for the activation of the reset button and user button on boards. Broadcom Pins on Raspberry PI used are pins 20 & 21 (GPIO Pins 38 & 40) which are used for user button and reset respectively.

This test utilizes the pigpio open source library, but the function utilized, `bsc_i2c` , has a delay when updating the slave buffer being sent to master. This delay, to be specific, is that it takes an i2c transaction between master and slave to update the slave buffer properly. This is a limitation of the RPi slave configuration, not the board being tested i2c configuration. To combat this, we utilize a dummy transaction that runs a i2c transaction of sending messages for 4 seconds to master from slave, then reopens a transaction (the transacton being tested) with the updated slave buffer properly timed. For more information about this delay and what exactly is happening to handle it, check the code `test.py` and the function `dummy_transaction`.

After Raspberry Pi initial boot, test will fail to receieve a message from device-to-test, due to pigpio daemon not booting properly. Run test again for better measurment after booting RPi.
