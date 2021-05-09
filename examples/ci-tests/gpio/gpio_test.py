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

        # Monitor variable
        self.prev_state = -1
        self.prev_toggle_time = -1
        self.toggle_period = -1

        # Test variable
        self.has_toggled = False

    def read_pin(self):
        """Reads input state and record toggle if a toggle of pin has been 
        observed.

        Argument:
        state (boolean) - Input state of pin
        """

        # Check current pin state
        if self.pin.value:
            if self.prev_state == 0:
                # Toggle appeared
                if self.prev_toggle_time > 0:
                    self.toggle_period = time.time() - self.prev_toggle_time
                    self.has_toggled = True

                self.prev_toggle_time = time.time()
                
            self.prev_state = 1

        else:
            if self.prev_state == 1:
                # Toggle appeared
                if self.prev_toggle_time > 0:
                    self.toggle_period = time.time() - self.prev_toggle_time
                    self.has_toggled = True

                self.prev_toggle_time = time.time()
            
            self.prev_state = 0

    def reset(self):
        """Reset will only reset value of toggle_period by design"""
        self.toggle_period = -1

    def is_toggled(self):
        """Check if the pin has been toggled in the past cycle"""
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
    def within_margin_error(self, value, target, margin_err):
        """Check if the value is within the given margin of error"""
        return value < target + margin_err and value > target - margin_err

    # Since the test toggles pin 0 every 1 second, the tester shall poll the pin
    # state twice the output rate to capture the changes.
    def test_sync_toggle(self):
        """Monitoring pin state every 500 ms
        
        Note: Since the test toggles pin every 1 second, the tester shall poll
        the pin state twice the output rate to capture the changes.
        """
        # Change line from docs to logging info
        print()

        TEST_CYCLE_TIME = 8
        EXPECTED_TIME_PERIOD = 1
        MOE = 0.01
        MAX_PIN_NO = 5

        for pin_no in range(0, MAX_PIN_NO):
            # If pin exists
            if hasattr(self, f'P{pin_no}'):
                pin_wrapper = getattr(self, f'P{pin_no}')

                # Monitor pin over cycle count
                for n in range(TEST_CYCLE_TIME):

                    # Log pin state
                    logger.info(
                        (f'P{pin_no} state: high' 
                        if pin_wrapper.pin.value 
                        else f'P{pin_no} state: low'),
                        extra={'timegap': time_gap(TEST_START_TIME)})
                
                    pin_wrapper.read_pin()

                    # If pin just toggled
                    if pin_wrapper.is_toggled():
                        logger.info(
                            (f'P{pin_no} state toggled in ' +
                            f'{pin_wrapper.toggle_period}s'),
                            extra={'timegap': time_gap(TEST_START_TIME)})

                        # ASSERT toggle period around 1 second
                        self.assertTrue(
                            self.within_margin_error(pin_wrapper.toggle_period,
                                                    EXPECTED_TIME_PERIOD,
                                                    MOE))

                        pin_wrapper.reset()

                    # End of cycle, go to sleep
                    time.sleep(0.5)

                # Ensure pin has toggled
                self.assertTrue(pin_wrapper.has_toggled)
            

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