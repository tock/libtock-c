# On Boot Up Scripts

## Run Python i2cboot

**Important:** This script is used to solve the cold-start problem the raspberry pi has on boot when running as slave. Currently, after booting the pi, the first i2c master test on a board (tests that require the RPi to act as a slave) will have an empty buffer, causing test to fail. However, after the first test script for master rx/tx is executed, the tests will now pass and buffer is filled properly. To solve this issue, we run this script on boot for the RPi whenever logged in from ssh to run the initial transaction for testing scripts to pass when necessary and not be affected by this cold-start issue. 

`i2cboot.py` provides basic test that sends message from slave to master, and waits for master to send message containing the slave message. This is essentially a Rx/Tx test, but we are primarily focusing on the reception of master here. The message sent from master will be printed over a 4 second period. If message sent from master is the exact same from message sent from slave, the script will pass - but this does not matter (Expected to fail)

To run the test,
```bash
sudo python3 i2cboot.py
```

Boards supported.

Board |
------|
nrf52840dk |

**NOTE**
This script requires the Raspberry Pi to be set as slave. Thus, you must use Broadcom Pins 10 & 11 (SDA & SCL resepectively) or GPIO pin 19 and 23. You must also download the open source python library "pigpio" which enables slave access on the Raspberry Pi. How to download the library is done here on **CI Hardware Documentation - Raspberry Pi setup**

This script requires button activation on tested boards. Two jumpers are required for the activation of the reset button and user button on boards. Broadcom Pins on Raspberry PI used are pins 20 & 21 (GPIO Pins 38 & 40) which are used for user button and reset respectively.

This test utilizes the pigpio open source library, but the function utilized, `bsc_i2c` , has a delay when updating the slave buffer being sent to master. This delay, to be specific, is that it takes an i2c transaction between master and slave to update the slave buffer properly. This is a limitation of the RPi slave configuration, not the board being tested i2c configuration. To combat this, we utilize a dummy transaction that runs a i2c transaction of sending messages for 4 seconds to master from slave, then reopens a transaction (the transacton being tested) with the updated slave buffer properly timed. For more information about this delay and what exactly is happening to handle it, check the code `i2cboot.py` and the function `dummy_transaction`.

