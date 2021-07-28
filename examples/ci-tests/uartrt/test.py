import serial
import time
import random
import struct

TARGET_ACKNOWLEDGEMENT = ""

sp = serial.Serial(port="/dev/ttyACM0", baudrate=115200, bytesize=8, timeout=2)
ser = serial.Serial(port="/dev/ttyS0", baudrate=115200, bytesize=8, parity="N", stopbits=1);
print("Starting Uart Rx/TX Test...\n")
#while(1):
#    c = random.randint(65, 90)
#    message = chr(c)
#    ser.write(struct.pack('<H', c))
#
#    
#    time.sleep(5)
#    if(sp.in_waiting > 0):
#        # print("A")
#        print("Message sent: " + message)
#        #time.sleep(5)
#        sp.readline()
#        if(sp.in_waiting > 0):
#            message_received = sp.readline()
#            message_received = message_received.decode("Ascii")
#            print("Message: " + message_received)
#            char_received = message_received[-1]
#            print("Echoed: " + char_received + "\n")
#            if(char_received == message):
#               print("Correct Serial Communication Message Received")
#                break
#       

#print("Uart Rx/Tx Test Passes")
#sp.close()
#ser.close()
#sp.open()
#ser.open()
#time.sleep(4)
#ser.write(b"yeah")

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
    def test_uart_rx_tx:
        while(1):
            c = random.randint(65, 90)
            TARGET_ACKNOWLEDGEMENT = chr(c)
            ser.write(struct.pack('<H', c))
            
            logger.info('UART Transaction Beginning',
                extra={'timegap': time_gap(TEST_START_TIME)})
    
            time.sleep(5)
            if(sp.in_waiting > 0):
                # print("A")
                logger.info('Message sent: ' + TARGET_ACKNNOWLEDGEMENT,
                           extra={'timegap': time_gap(TEST_START_TIME)})
                #time.sleep(5)
                sp.readline()
                if(sp.in_waiting > 0):
                    message_received = sp.readline()
                    message_received = message_received.decode("Ascii")
                    #print("Message: " + message_received)
                    char_received = message_received[-1]
                    #print("Echoed: " + char_received + "\n")
                    if(char_received == TARGET_ACKNOWLEDGEMENT):
                        print("Message Received (r[character]): " + message_received)
                        print("Echoed: " + char_received + "\n")
                        print("Correct Serial Communication Message Received")
                        self.assertTrue(true)
                        break
        
# END

################################################################################
# Test Case Setup
################################################################################


class Nrf52840Test(UartTest):
    def setUp(self):
        logger.info('Setting up for nrf52840dk Uart Rx/Tx test...',
            extra={'timegap': time_gap(TEST_START_TIME)})

# END

################################################################################
# Main
################################################################################

if __name__ == '__main__':
    unittest.main()
