# I2C UART Rx/Tx Test
# This tester corresponds to libtock-c/examples/ci-tests/uart-rx-tx test.

import logging
import unittest
import serial
import time
import random
import struct

TARGET_ACKNOWLEDGEMENT = ""
sp = None       # Serial port for specific boards to read the echo of messages
ser = serial.Serial(port="/dev/ttyS0", baudrate=115200, bytesize=8, parity="N", stopbits=1)


print("Starting Uart Rx/TX Test...\n")

################################################################################
# Helper Functions
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
logger = logging.getLogger('UART Test')
logger.setLevel('INFO')

logger.info('Initiating UART test...',
            extra={'timegap': time_gap(TEST_START_TIME)})

# END

################################################################################
# Test Case Module
################################################################################

class UartTest(unittest.TestCase):
    def test_uart_rx_tx(self):
        while(1):
            logger.info('Waiting for message...',
                extra={'timegap': time_gap(TEST_START_TIME)})
            
            buffer_period = float(time_gap(TEST_START_TIME))
            if(buffer_period > 45.0):
                sp.close()
                ser.close()
                self.assertTrue(False)
            
            c = random.randint(65, 90)
            TARGET_ACKNOWLEDGEMENT = chr(c)
            ser.write(struct.pack('<H', c))
            
            logger.info('UART Transaction Beginning',
                extra={'timegap': time_gap(TEST_START_TIME)})
    
            time.sleep(5)
            if(sp.in_waiting > 0):

                logger.info('Message sent: ' + TARGET_ACKNOWLEDGEMENT,
                           extra={'timegap': time_gap(TEST_START_TIME)})
                sp.readline()
                if(sp.in_waiting > 0):

                    message_received = sp.readline()
                    message_received = message_received.decode("Ascii")
                    char_received = message_received[-1]
                    if(char_received == TARGET_ACKNOWLEDGEMENT):

                        logger.info("Message Received (r[character]): " + message_received,
                                   extra={'timegap': time_gap(TEST_START_TIME)})
                        logger.info("Echoed: " + char_received,
                                   extra={'timegap': time_gap(TEST_START_TIME)})
                        print("Correct Serial Communication Message Received")
                        self.assertTrue(True)
                        sp.close()
                        ser.close()
                        break
        
# END

################################################################################
# Test Case Setup
################################################################################


class Nrf52840Test(UartTest):
    def setUp(self):
        global sp

        sp = serial.Serial(port="/dev/ttyACM0", baudrate=115200, bytesize=8, timeout=2)
        logger.info('Setting up for nrf52840dk Uart Rx/Tx test...',
            extra={'timegap': time_gap(TEST_START_TIME)})

class HailTest(UartTest):
    def setUp(self):
        global sp

        sp = serial.Serial(port="/dev/ttyUSB0", baudrate=115200, bytesize=8, parity="N", stopbits=1)
        logger.info('Setting up for hail Uart Rx/Tx test...',
            extra={'timegap': time_gap(TEST_START_TIME)})

# END

################################################################################
# Main
################################################################################

if __name__ == '__main__':
    unittest.main()
