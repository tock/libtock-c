# BLE Test
# This tester corresponds to libtock-c/examples/ci-tests/ble test.

import logging
import time
import unittest
import os
import subprocess

TARGET_NAME = 'TockOS'

################################################################################
# Helper classes and functions
################################################################################

def time_gap(start_time):
    """Return time gap between current time and start_time

    Argument:
    start_time - Start time
    """
    return "{:.6f}".format(time.time() - start_time)

# END

################################################################################
# Start test and logger
################################################################################

# Test Start Time
TEST_START_TIME = time.time()

# Logger set format
LOG_FORMAT = "%(timegap)s %(levelname)s -- %(message)s"
logging.basicConfig(format=LOG_FORMAT)

# Logger add formatter
logger = logging.getLogger('BLE Test')
logger.setLevel('INFO')

logger.info('Initiating BLE test...',
            extra={'timegap': time_gap(TEST_START_TIME)})

# END

################################################################################
# Test Case Module
################################################################################

class BleTest(unittest.TestCase):
    def test_ble_advertise(self):
        """Check if the advertised device name can be found"""
        # Change line from docs to logging info
        print()

        logger.info('Bluetooth Status on RPi Harness\n',
            extra={'timegap': time_gap(TEST_START_TIME)})
        os.system('sudo systemctl status bluetooth')
        print() # Line change
        os.system('sudo systemctl status hciuart')
        print() # Line change
        # os.system('sudo timeout 5 stdbuf -oL hcitool lescan')
        # print() # Line change
        scan_cmd = 'sudo timeout 5 stdbuf -oL hcitool lescan'

        # BLE scan flag
        found = False

        try:
            scan_result = subprocess.check_output(scan_cmd, 
                                              stderr=subprocess.STDOUT,
                                              shell=True)
            scan_result_str = scan_result.decode('ascii')
            logger.info('Scan result:\n' + scan_result_str,
                        extra={'timegap': time_gap(TEST_START_TIME)})
            
            # Search for target board name
            scan_entries = scan_result_str.split('\n')
            print(scan_entries)

            for entry in scan_entries:
                mac_addr, name = entry.split(' ')
                print(name)

                if name == TARGET_NAME:
                    found = True

        except subprocess.CalledProcessError as err:
            # Print error
            scan_result_str = err.output.decode('ascii')

            logger.info('Scan result error:\n' + scan_result_str,
                        extra={'timegap': time_gap(TEST_START_TIME)})

            # Search for target board name
            scan_entries = scan_result_str.split('\n')

            for entry in scan_entries:
                if entry != '':
                    mac_addr, name = entry.split(' ', 1)

                    if name == TARGET_NAME:
                        found = True

        finally:
            logger.info('BLE scan ended.',
                extra={'timegap': time_gap(TEST_START_TIME)})

            # Restart bluetooth
            # Note: the scanning process is corrupted whenever we try to kill it, so
            #       for now, we resort to restarting bluetooth every test, but if
            #       there is a better implementation, feel free to change this.
            logger.info('Restarting test harness bluetooth.',
                extra={'timegap': time_gap(TEST_START_TIME)})
            os.system('sudo hciconfig hci0 down; sudo hciconfig hci0 up')

            self.assertTrue(found)

# END

################################################################################
# Test Case Setup
################################################################################

class Nrf52840Test(BleTest):
    def setUp(self):
        logger.info('Setting up for nrf52840dk BLE test...',
            extra={'timegap': time_gap(TEST_START_TIME)})

# END

################################################################################
# Main
################################################################################

if __name__ == '__main__':
    unittest.main()
