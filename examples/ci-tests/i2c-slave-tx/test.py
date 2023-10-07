# I2C Slave Tx Test
# This tester corresponds to libtock-c/examples/ci-tests/i2c-slave-tx test.

from smbus import SMBus
import time
import os
import logging
import unittest
from gpiozero import InputDevice
from gpiozero import OutputDevice

RESET = 21 # Broadcom pin 21 (P1 pin 40)
BUTTON_1 = 20 # Broadcom pin 20 (P1 pin 38)

button = OutputDevice(BUTTON_1)
reset_button = OutputDevice(RESET)

ADDRESS = 0x41 # bus address to the Slave Device
MASTER = 0x40 # Raspberry Pi Master Address
MESSAGE = "Hello friend." # Message sent from slave
BUF_SIZE = 16
bus = SMBus(1) # indicates /dev/ic2-1

# cancelling slave configuration pins
pin_1 = InputDevice(10)
pin_2 = InputDevice(11)

################################################################################
# Helper Functions
################################################################################

def time_gap(start_time):
    """Return time gap between current time and start_time
    Argument:
    start_time - Start time
    """
    return "{:.6f}".format(time.time() - start_time)

def reset():
   global RESET
   """Button is Reset"""

   reset_button.off()
   time.sleep(1.1)
   reset_button.on()
   time.sleep(0.5)

def press_button():
   global BUTTON_1
   """Button is one of User Buttons"""

   button.on()
   time.sleep(1.1)
   button.off()
   time.sleep(0.5)

def message_decoder(data):
    string = ''
    for item in data:
        string += chr(item)
    return string

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
logger = logging.getLogger('I2C Slave Tx Test')
logger.setLevel('INFO')

logger.info('Initiating I2C Slave Tx test...',
            extra={'timegap': time_gap(TEST_START_TIME)})

# END

################################################################################
# Test Case Module
################################################################################

class I2CSlaveTxTest(unittest.TestCase):
    def test_i2c_slave_tx_configuration(self):
        
        print()
        logger.info('Communicating with I2C Device to Receive Message: ' + MESSAGE,
            extra={'timegap': time_gap(TEST_START_TIME)})
        
        received = False

        time.sleep(2)  # Wait until app has fully reset

        press_button() # Used to press one of the user buttons on the board
        
        try:
            dataM = bus.read_i2c_block_data(ADDRESS, MASTER, BUF_SIZE)
            message = message_decoder(dataM)
            message = str(message)
            message_stripped = message.splitlines()
            message_received = message_stripped[0]

            if(MESSAGE == message_received):
            
                logger.info('Message Received: ' + message_received,
                            extra={'timegap': time_gap(TEST_START_TIME)})

                time.sleep(1)

                logger.info('Message Sent Successfully from Slave\n',
                            extra={'timegap': time_gap(TEST_START_TIME)})

                logger.info('Connection Satisfied.',
                extra={'timegap': time_gap(TEST_START_TIME)})

                time.sleep(1)

                received = True
            
        except OSError as err:
            print("OS error: {0}".format(err))

            logger.info('Test failed...',
                        extra={'timegap': time_gap(TEST_START_TIME)})
            print()

            time.sleep(1)
        except TimeoutError:
            logger.info('Connection is poor: Time out error...\n',
                        extra={'timegap': time_gap(TEST_START_TIME)})

            logger.info('Test failed...',
                        extra={'timegap': time_gap(TEST_START_TIME)})
            print()

            time.sleep(1)
        finally:
            logger.info('I2C Communication Ended...',
                        extra={'timegap': time_gap(TEST_START_TIME)})

            reset()      # Reset application to stop sending messages

            bus.close()

            time.sleep(1)

            logger.info('I2C Slave Tx Test has ended.\n',
                extra={'timegap': time_gap(TEST_START_TIME)})
            
            self.assertTrue(received)
# END

################################################################################
# Test Case Setup
################################################################################

class Nrf52840Test(I2CSlaveTxTest):
    def setUp(self):
        reset() # Used to activate the reset button on board

        logger.info('Setting up for nrf52840dk I2C Slave Tx test...',
            extra={'timegap': time_gap(TEST_START_TIME)})


# END

################################################################################
# Main
################################################################################

if __name__ == '__main__':
    unittest.main()
