# BLE Scan Test

## Test Description

`test.py` provides the following tests:

### BleTest

This test utilizes the test harness (RPi) capabilities to run a ble scan of the device-under-test to verfiy ble connectivity works as intended. The test harness simply runs a scan of all bluetooth devices in the vacinity, and locates the device-under-test (tock device) for ble connection.

## Example
     0.000152 INFO -- Initiating BLE test...
     0.016245 INFO -- Setting up for nrf52840dk BLE test...
     0.016528 INFO -- Bluetooth Status on RPi Harness

     ● bluetooth.service - Bluetooth service
         Loaded: loaded (/lib/systemd/system/bluetooth.service; enabled; vendor preset: enabled)
         Active: active (running) since Fri 2021-08-27 18:17:17 UTC; 4min 38s ago
         Docs: man:bluetoothd(8)
     Main PID: 1912 (bluetoothd)
         Status: "Running"
         Tasks: 1 (limit: 973)
         CGroup: /system.slice/bluetooth.service
                  └─1912 /usr/lib/bluetooth/bluetoothd

     Aug 27 18:17:17 ubuntu systemd[1]: Starting Bluetooth service...
     Aug 27 18:17:17 ubuntu bluetoothd[1912]: Bluetooth daemon 5.53
     . (other information..)
     .
     .
     Aug 27 18:17:17 ubuntu systemd[1]: Started Configure Bluetooth Modems connected by UART.
     6.719571 INFO -- Scan result error:
     LE Scan ...
     08:99:10:53:EB:97 (unknown)
     40:8E:59:DA:D9:F0 (unknown)
     F0:00:00:00:02:F0 TockOS
     65:83:87:B5:B6:22 (unknown)
     . (more devices..)
     .
     .

     6.720389 INFO -- BLE scan ended.
     6.720653 INFO -- Restarting test harness bluetooth.
     .
     ----------------------------------------------------------------------
     Ran 1 test in 6.899s


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
