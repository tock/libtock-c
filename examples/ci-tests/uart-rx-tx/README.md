# UART Rx/Tx CI Test

## Test Description

`test.py` provides the following tests:

### UartTest

This test sends a character from RPi to device-under-test through uart communication, then device-under-test receives the character and sends the character back to RPi to verify correct character was sent.

  1. The RPi generates a random character, and sends the data over through Uart communication of the RPi Uart Tx pin.
  1. The device-under-test receives data through Uart connection (USB or Uart Rx), then creates new message as `"r(character)"` for test-harness verify that it is the return message of device-under-test
  1. The test harness then strips message to get the character, and validates that the data received matches the expected character transmitted.
  
## Example Output
     Starting Uart Rx/TX Test...

     0.000187 INFO -- Initiating UART test...
     0.016512 INFO -- Setting up for nrf52840dk Uart Rx/Tx test...
     0.016805 INFO -- Waiting for message...
     0.017386 INFO -- UART Transaction Beginning
     5.022696 INFO -- Waiting for message...
     5.023263 INFO -- UART Transaction Beginning
     10.028633 INFO -- Message sent: R
     12.031758 INFO -- Message Received (r[character]): rR
     12.032129 INFO -- Echoed: R
     Correct Serial Communication Message Received
     .
     ----------------------------------------------------------------------
     Ran 1 test in 12.021s

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
