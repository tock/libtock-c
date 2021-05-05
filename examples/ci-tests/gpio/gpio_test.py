# gpio_test.py
# This tester corresponds to libtock-c/examples/ci-tests/gpio test.

import logging
import time
import unittest
from gpiozero import InputDevice

def time_gap(start_time):
    """Return time gap between current time and start_time

    Argument:
    start_time - Start time
    """
    return "{:.6f}".format(time.time() - start_time)

class Pin:
    """Pin object"""
    def __init__(self, pin_no, pin_name = None):
        # Set initial variables to invalid value
        self.pin = InputDevice(pin_no)
        self.prev_state = -1
        self.prev_toggle_time = -1
        self.toggle_period = -1

    def read_pin(self):
        """Reads input state and record toggle if a toggle of pin has been 
        observed.

        Argument:
        state (boolean) - Input state of pin
        """
        if self.is_toggled():
            self.toggle_period = -1
            self.prev_toggle_time = -1

        if self.pin.value:
            if self.prev_state == 0:
                # Toggle appeared
                if self.prev_toggle_time > 0:
                    self.toggle_period = time.time() - self.prev_toggle_time
                else:
                    self.prev_toggle_time = time.time()
                
            self.prev_state = 1

        else:
            if self.prev_state == 1:
                # Toggle appeared
                if self.prev_toggle_time > 0:
                    self.toggle_period = time.time() - self.prev_toggle_time
                else:
                    self.prev_toggle_time = time.time()
            
            self.prev_state = 0

    def is_toggled(self):
        """Return if the toggle has been recorded"""
        return self.toggle_period > 0

################################################################################
# Start test and logger
################################################################################

# Test Start Time
TEST_START_TIME = time.time()

# Logger set format
LOG_FORMAT = "%(timegap)s %(levelname)s -- %(message)s"
logging.basicConfig(format=LOG_FORMAT)

# Logger add formatter
logger = logging.getLogger('GPIO Test')
logger.setLevel('INFO')

logger.info('Initiating GPIO test...',
            extra={'timegap': time_gap(TEST_START_TIME)})

# END

################################################################################
# Test Cases
################################################################################

class GpioTest(unittest.TestCase):
    # Since the test toggles pin 0 every 1 second, the tester shall poll the pin
    # state twice the output rate to capture the changes.
    def test_toggle(self):
        """Monitoring pin state every 500 ms
        
        Note: Since the test toggles pin every 1 second, the tester shall poll
        the pin state twice the output rate to capture the changes.
        """
        # Change line from docs to logging info
        print('\n')

        for n in range(8):
            logger.info(
                ('P0 state: high' 
                if self.P0.pin.value 
                else 'P0 state: low'),
                extra={'timegap': time_gap(TEST_START_TIME)})
            self.P0.read_pin()
            if self.P0.is_toggled():
                logger.info(f'D0 state toggled in {self.P0.toggle_period}s',
                            extra={'timegap': time_gap(TEST_START_TIME)})

            time.sleep(0.5)

class Nrf52840GpioTest(GpioTest):
    def setUp(self):
        # Assign input pins
        self.P0 = Pin(5)
        self.P1 = Pin(6)
        self.P2 = Pin(13)
        self.P3 = Pin(19)
        self.P4 = Pin(26)

if __name__ == '__main__':
    unittest.main()