from smbus import SMBus
import time
import logging
import unittest

ADDRESS = 0x41 # bus address to the Slave Device
MASTER = 0x40 # Raspberry Pi Master Address
MESSAGE = "Hello I'm Master" # Message to send to slave
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

def message_converter(message):
    """Return list of ascii values for each character in message
    Argument:
    string I2C_message
    """
    encoded = []                    # Encoded message for ascii values
    chars = list(message)           # Spliting the string message into characters
    
    for items in chars:
        encoded.append(ord(items))  # Filling encoded with ascii values of characters
    
    return encoded

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
    def test_i2c_slave_rx_configuration(self):
        
        print()
        logger.info('Sending I2C Message: ' + MESSAGE,
            extra={'timegap': time_gap(TEST_START_TIME)})
        
        received = False
        
        message_to_send = message_converter(MESSAGE)

        time.sleep(1)
        try:
            bus.write_i2c_block_data(ADDRESS, MASTER, message_to_send)
            
            logger.info('Message Sent Sucessfully.\n',
                        extra={'timegap': time_gap(TEST_START_TIME)})
            time.sleep(1)
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

            # Close Setup

            bus.close()

            time.sleep(1)

            logger.info('Connection Satisfied.',
                extra={'timegap': time_gap(TEST_START_TIME)})
            received = True

            time.sleep(1)

            logger.info('I2C Slave Rx Test has ended.',
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

# END

################################################################################
# Main
################################################################################

if __name__ == '__main__':
    unittest.main()
