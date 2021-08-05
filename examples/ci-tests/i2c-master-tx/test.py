import time
import logging
import unittest
import RPi.GPIO as GPIO
import os

MESSAGE = 'Hello friend.' # Message Master sends to slave
MESSAGE_RECEIVED = ''
FIRST_RX = 0

SDA = 10 # Broadcom pin 10 (P1 pin 19)
SCL = 11 # Broadcom pin 11 (P1 pin 23)
RESET = 21 # Broadcom pin 21 (P1 pin 40)
BUTTON_1 = 20 # Broadcom pin 20 (P1 pin 38)

I2C_ADDR = 0x41 # Raspberry Pi Address

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(RESET, GPIO.OUT) # RESET pin set as output
GPIO.setup(BUTTON_1, GPIO.OUT) # BUTTON_1 pin set as output

# Set up PiGPIO properly by configuring it on pi then importing library
os.system('sudo pigpiod')
import pigpio

pi = pigpio.pi() # Configure the Raspberry Pi as slave
# PiGPIO configured.

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

def i2c(id, tick):
   global pi
   global FIRST_RX
   global MESSAGE_RECEIVED
   global I2C_ADDR

   s, b, d = pi.bsc_i2c(I2C_ADDR)

   if b:
      array = str(d[:-1])
      logger.info('Messsage Received: ' + array,
            extra={'timegap': time_gap(TEST_START_TIME)})
      
      if(FIRST_RX < 1):
         string_received = d.decode()
         string_split = string_received.splitlines()
         MESSAGE_RECEIVED = string_split[0]
         FIRST_RX += 1

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
logger = logging.getLogger('I2C Master Tx Test')
logger.setLevel('INFO')

logger.info('Initiating I2C Master Tx Test...',
            extra={'timegap': time_gap(TEST_START_TIME)})

# END

################################################################################
# Test Case Module
################################################################################

class I2CMasterTxTest(unittest.TestCase):
    def test_i2c_master_tx_configuration(self):
        """ Set up for Raspberry Pi configuration """
        global pi
        global MESSAGE_RECEIVED

        print()
        logger.info('Receiving Message As Slave... ',
            extra={'timegap': time_gap(TEST_START_TIME)})
        
        received = False

        if not pi.connected:
            exit()

        press_button()          # Used to press on of the user buttons on the board

        # Add pull-ups in case external pull-ups haven't been added (For Raspberry Pi)

        pi.set_pull_up_down(SDA, pigpio.PUD_UP)
        pi.set_pull_up_down(SCL, pigpio.PUD_UP)

        # Respond to BSC slave activity
        e = pi.event_callback(pigpio.EVENT_BSC, i2c)

        pi.bsc_i2c(I2C_ADDR)    # Configure BSC as I2C slave

        print()
        logger.info('Initiating Reception of Messages...',
            extra={'timegap': time_gap(TEST_START_TIME)})

        time.sleep(12)          # Time to wait for messages to be sent (Should see four messages logged)
        
        MESSAGE_RECEIVED = str(MESSAGE_RECEIVED)
        MESSAGE_RECEIVED = MESSAGE_RECEIVED.strip()

        logger.info('Expected Message: ' + MESSAGE,
            extra={'timegap': time_gap(TEST_START_TIME)})

        logger.info('Message Received: ' + MESSAGE_RECEIVED,
            extra={'timegap': time_gap(TEST_START_TIME)})

        if (MESSAGE_RECEIVED == MESSAGE):

            # Close setup
            e.cancel()

            pi.bsc_i2c(0) # Disable BSC peripheral

            GPIO.cleanup()

            pi.stop()

            reset()      # Reset application to stop sending messages

            time.sleep(1)

            logger.info('Connection Satisfied.',
                extra={'timegap': time_gap(TEST_START_TIME)})
            received = True

            time.sleep(1)

            logger.info('I2C Master Tx Test has ended.',
                extra={'timegap': time_gap(TEST_START_TIME)})
            self.assertTrue(received)

        else:
            
            #Close setup

            e.cancel()

            pi.bsc_i2c(0)   # Disable BSC peripheral

            GPIO.cleanup()

            pi.stop()

            reset()         # Reset application to stop sending messages

            time.sleep(1)

            logger.info('Connection was not Satisfied. Wrong/No message received.',
                extra={'timegap': time_gap(TEST_START_TIME)})

            time.sleep(1)

            logger.info('I2C Master Tx Test has ended.',
                extra={'timegap': time_gap(TEST_START_TIME)})
            self.assertTrue(received)


# END

################################################################################
# Test Case Setup
################################################################################

class Nrf52840Test(I2CMasterTxTest):
    def setUp(self):
        
        reset()                 # Used to activate the reset button on board

        logger.info('Setting up for nrf52840dk I2C Master Tx test...',
            extra={'timegap': time_gap(TEST_START_TIME)})
            

# END

################################################################################
# Main
################################################################################

if __name__ == '__main__':
    unittest.main()