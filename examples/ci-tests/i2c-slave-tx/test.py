from smbus import SMBus
import time
import os
import logging
import unittest
import RPi.GPIO as GPIO

RESET = 21 # Broadcom pin 21 (P1 pin 40)
BUTTON_1 = 20 # Broadcom pin 20 (P1 pin 38)

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(RESET, GPIO.OUT) # RESET pin set as output
GPIO.setup(BUTTON_1, GPIO.OUT) # BUTTON_1 pin set as output

ADDRESS = 0x41 # bus address to the Slave Device
MASTER = 0x40 # Raspberry Pi Master Address
MESSAGE = "Hello friend." # Message sent from slave
BUF_SIZE = 16
bus = SMBus(1) # indicates /dev/ic2-1

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

   GPIO.output(RESET, GPIO.LOW)
   time.sleep(1)
   GPIO.output(RESET, GPIO.HIGH)

def press_button():
   global BUTTON_1
   """Button is one of User Buttons"""

   GPIO.output(BUTTON_1, GPIO.HIGH)
   time.sleep(1)
   GPIO.output(BUTTON_1, GPIO.LOW)

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
logger = logging.getLogger('I2C Rx Test')
logger.setLevel('INFO')

logger.info('Initiating I2C Rx test...',
            extra={'timegap': time_gap(TEST_START_TIME)})

# END

################################################################################
# Test Case Module
################################################################################

class I2CRxTest(unittest.TestCase):
    def test_i2c_slave_configuration(self):
        
        print()
        logger.info('Communicating with I2C Device to Receive Message: ' + MESSAGE,
            extra={'timegap': time_gap(TEST_START_TIME)})
        
        received = False

        press_button()
        
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
                received = True
            
        except OSError:
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

            # Close Setup
            GPIO.cleanup()

            bus.close()

            time.sleep(1)

            logger.info('Connection Satisfied.',
                extra={'timegap': time_gap(TEST_START_TIME)})
            received = True

            time.sleep(1)

            logger.info('I2C Slave Tx Test has ended.\n',
                extra={'timegap': time_gap(TEST_START_TIME)})
            
            self.assertTrue(received)
# END

################################################################################
# Test Case Setup
################################################################################

class Nrf52840Test(I2CRxTest):
    def setUp(self):
        logger.info('Setting up for nrf52840dk I2C Rx test...',
            extra={'timegap': time_gap(TEST_START_TIME)})

        reset()

# END

################################################################################
# Main
################################################################################

if __name__ == '__main__':
    unittest.main()