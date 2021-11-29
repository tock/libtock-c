# I2C Master Rx Test
# This tester corresponds to libtock-c/examples/ci-tests/i2c-master-rx test.

import time
import logging
import unittest
from gpiozero import InputDevice
from gpiozero import OutputDevice
import os

MESSAGE_SENT = "Hello I'm Slave" # Message Master sends to slave
MESSAGE_CONFIRMATION= ''
FIRST_RX = 0
dummy = False #Setup for dummy transaction (Buffer takes one transaction session to update properly)


SDA = 10 # Broadcom pin 10 (P1 pin 19)
SCL = 11 # Broadcom pin 11 (P1 pin 23)
RESET = 21 # Broadcom pin 21 (P1 pin 40)
BUTTON_1 = 20 # Broadcom pin 20 (P1 pin 38)

I2C_ADDR = 0x41 # Raspberry Pi Address

button = OutputDevice(BUTTON_1)
reset_button = OutputDevice(RESET)

# Set up PiGPIO properly by configuring it on pi then importing library
os.system('sudo pigpiod')
time.sleep(1)

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

def i2c(id, tick):
   global pi
   global FIRST_RX
   global MESSAGE_CONFIRMATION
   global I2C_ADDR
   global dummy

   s, b, d = pi.bsc_i2c(I2C_ADDR, b"\nHello I'm Slave\n")

   # Check if dummy transaction is occuring, if not, test has started.
   if not dummy:
      if b:
         if(FIRST_RX < 1):
           MESSAGE_CONFIRMATION = d.decode()
           FIRST_RX += 1

         array = str(d[:-1])
         logger.info('Messsage Call Back From Master: ' + array,
               extra={'timegap': time_gap(TEST_START_TIME)})


def dummy_transaction():
    global pi
    global I2C_ADDR
    global dummy
    """
    This function is used to conteract the update delay
    on the i2c slave buffer. The delay occurs when the buffer is
    updated and requires the bus to enact a transcation before the
    update to the buffer actually takes place. This function, then, 
    initiates that transaction to occur, and update the buffer in proper time.
    """

    dummy = True # Update to initiate the dummy transaction properly on the i2c function

    if not pi.connected:
            exit()

    press_button()

    # Add pull-ups in case external pull-ups haven't been added (For Raspberry Pi)

    pi.set_pull_up_down(SDA, pigpio.PUD_UP)
    pi.set_pull_up_down(SCL, pigpio.PUD_UP)

    # Respond to BSC slave activity
    e = pi.event_callback(pigpio.EVENT_BSC, i2c)

    pi.bsc_i2c(I2C_ADDR)

    time.sleep(4)

    e.cancel()

    pi.bsc_i2c(0)

    reset()

    dummy = False # End dummy transaction, so proper testing is conducted on i2c function

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
logger = logging.getLogger('I2C Master Rx Test')
logger.setLevel('INFO')

logger.info('Initiating I2C Master Rx Test...',
            extra={'timegap': time_gap(TEST_START_TIME)})

# END

################################################################################
# Test Case Module
################################################################################

class I2CMasterRxTest(unittest.TestCase):
    def test_i2c_master_rx_configuration(self):
        """ Set up for Raspberry Pi configuration """
        global pi
        global MESSAGE_CONFIRMATION

        dummy_transaction() # Initiate the dummy transaction to update buffer in proper time

        print()
        logger.info('Sending Messages As Slave... ',
            extra={'timegap': time_gap(TEST_START_TIME)})

        received = False

        if not pi.connected:
            exit()

        time.sleep(2)           # Wait until app has fully reset

        press_button()          # Used to press one of the user buttons on the board

        # Add pull-ups in case external pull-ups haven't been added (For Raspberry Pi)

        pi.set_pull_up_down(SDA, pigpio.PUD_UP)
        pi.set_pull_up_down(SCL, pigpio.PUD_UP)

        # Respond to BSC slave activity
        e = pi.event_callback(pigpio.EVENT_BSC, i2c)

        pi.bsc_i2c(I2C_ADDR)    # Configure BSC as I2C slave

        print()
        logger.info('Initiating Transmission of Messages...',
            extra={'timegap': time_gap(TEST_START_TIME)})

        time.sleep(12)          # Time to wait for messages to be sent (Should see four messages logged)
        
        MESSAGE_CONFIRMATION = str(MESSAGE_CONFIRMATION)
        MESSAGE_CONFIRMATION = MESSAGE_CONFIRMATION.strip()

        logger.info('Message Sent: ' + MESSAGE_SENT,
            extra={'timegap': time_gap(TEST_START_TIME)})

        logger.info('Message Called Back from Master: ' + MESSAGE_CONFIRMATION,
            extra={'timegap': time_gap(TEST_START_TIME)})

        if (MESSAGE_CONFIRMATION == MESSAGE_SENT):

            # Close setup
            e.cancel()

            pi.bsc_i2c(0)   # Disable BSC peripheral

            pi.stop()

            reset()         # Reset application to stop sending messages

            time.sleep(1)

            logger.info('Connection Satisfied.',
                extra={'timegap': time_gap(TEST_START_TIME)})

            received = True

            time.sleep(1)

            logger.info('I2C Master Rx Test has ended.',
                extra={'timegap': time_gap(TEST_START_TIME)})

            os.system('sudo killall pigpiod')
            time.sleep(1)

            self.assertTrue(received)

        else:

            #Close setup

            e.cancel()

            pi.bsc_i2c(0)   # Disable BSC peripheral

            pi.stop()

            reset()         # Reset application to stop sending messages

            time.sleep(1)

            logger.info('Connection was not Satisfied. Wrong/No message was sent.',
                extra={'timegap': time_gap(TEST_START_TIME)})

            time.sleep(1)

            logger.info('I2C Master Rx Test has ended.',
                extra={'timegap': time_gap(TEST_START_TIME)})

            os.system('sudo killall pigpiod')
            time.sleep(1)

            self.assertTrue(received)


# END

################################################################################
# Test Case Setup
################################################################################

class Nrf52840Test(I2CMasterRxTest):
    def setUp(self):
        
        reset()                 # Used to activate the reset button on board

        logger.info('Setting up for nrf52840dk I2C Master Rx test...',
            extra={'timegap': time_gap(TEST_START_TIME)})
            

# END

################################################################################
# Main
################################################################################

if __name__ == '__main__':
    unittest.main()
