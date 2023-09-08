# I2C Slave RX Test

## Test Description

`test.py` provides the following tests:

## I2CSlaveRxTest

This receives a message from the device-under-test as I2C slave from the RPi as I2C master.

  1. The device-under-test configures itself as I2C slave with address 0x41 and awaits for the RPi to initiate an I2C transaction.
  1. The test harness presses sends a message on the bus to initiate the transaction.
  1. The test harness issues an I2C write transaction of 17 bytes to address 0x41.
  1. The test harness validates that transaction occured if the message was sent and acknowledged by the device-under-test through the function used. 

## Example Output

      0.000200 INFO -- Initiating I2C Rx test...
      0.015161 INFO -- Setting up for nrf52840dk I2C Slave Rx test...

      0.015468 INFO -- Sending I2C Message: Hello I'm Master
      1.018435 INFO -- Message Sent Sucessfully.

      2.019948 INFO -- I2C Communication Ended...
      3.021490 INFO -- Connection Satisfied.
      4.023019 INFO -- I2C Slave Rx Test has ended.
      .
      ----------------------------------------------------------------------
      Ran 1 test in 4.009s

      OK

## Notes

When manually invoking the test harness, you must specify the board under test.
Otherwise, python unittest will attempt to run tests for all platforms.

To run the test,
```bash
sudo python3 test.py Nrf52840Test
```

### Supported Boards

CI has been validated and runs on the following hardware platforms:

Board | Test Name
------|----------
nrf52840dk | Nrf52840Test

### Test Configuration Notes

**NOTE**
This test requires the Raspberry Pi to be set as Master. Thus, you must use Broadcom Pins 2 & 3 (SDA & SCL resepectively) or GPIO pin 3 and 5. 