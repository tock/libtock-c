Hardware CI Tests
=================

This folder holds tests that run as part of Tock's
[hardware-in-the-loop CI](https://github.com/tock/tock/blob/master/doc/CI_Hardware.md).

These are applications that are automatically loaded onto test hardware.

Each test has an application which is loaded onto the Tock hardware platform
under test (`main.c`) and a test harness runner (`test.py`) which is run on the
supporting test-runner platform (currently expected to be a Raspberry Pi –
this is because the test runner must have low-level hardware access, i.e. GPIO,
I2C, SPI, etc support, and the test harnesses all use Raspberry Pi-specific
libraries to support this).

While these tests are written to be run by the automated CI infrastructure,
they should generally also be abe to be run standalone as well (again, if run
on a Raspberry Pi). The tests also assume physical pin connections (i.e. a wire
between a pin on the board under test and the driving Raspberry Pi header) as
documented in each board's test harness description in the main Tock repo.

To run a test, enter the test directory and invoke the unit test corresponding
to the hardware platform attached to the Raspberry Pi you are logged into:

    [rpi01] $ cd uart-rx-tx
    [rpi01] $ python3 test.py Nrf52840Test
    
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

Tests use the standard Python [`unittest`](https://docs.python.org/3/library/unittest.html) framework.

To the extent possible, tests SHOULD be hardware-agnostic. The only
per-platform specification should be pin assignment. Board configurations in
the main Tock repo specify supported hardware and tests. For this reason, if a
test attempts to run on a platform that inherently cannot support it (e.g. BLE
tests on a platform with no Bluetooth radio), the test MUST fail.

Tests may assume full ownership of the hardware. Each folder is a logical unit
of testing and is designed to be run in isolation. Currently, there are no
multitenant hardware-CI tests. If and when these are added, current thinking is
to put concurrent test configurations into a folder such that the folder
remains the atomic unit of a test, but this is certainly subject to change.
