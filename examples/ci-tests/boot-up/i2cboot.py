# I2C Master Rx Test
# This tester corresponds to libtock-c/examples/ci-tests/i2c-master-rx test.

from pathlib import Path
import time
import logging
import toml
from gpiozero import InputDevice
from gpiozero import OutputDevice
import os

CONFIG_FILE = f'{Path.home()}/tock/tools/tock-test-harness/config.toml'
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
time.sleep(2)

import pigpio
time.sleep(1)

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

def press_button():
   global BUTTON_1
   """Button is one of User Buttons"""

   button.on()
   time.sleep(1.5)
   button.off()

def reset():
   global RESET

   reset_button.on()
   time.sleep(1.1)
   reset_button.off()
   reset_button.toggle() # Set Pin to tri-state

def i2c(id, tick):
   global pi
   global FIRST_RX
   global MESSAGE_CONFIRMATION
   global I2C_ADDR
   global dummy

   s, b, d = pi.bsc_i2c(I2C_ADDR, b"\nHello I'm Slave\n")

   # Check if dummy transaction is occuring, if not, test has started.
   if dummy:
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

    if not pi.connected:
            exit()

    press_button()

    # Add pull-ups in case external pull-ups haven't been added (For Raspberry Pi)

    pi.set_pull_up_down(SDA, pigpio.PUD_UP)
    pi.set_pull_up_down(SCL, pigpio.PUD_UP)

    # Respond to BSC slave activity
    e = pi.event_callback(pigpio.EVENT_BSC, i2c)

    pi.bsc_i2c(I2C_ADDR)

    time.sleep(3)

    # Reset Operation
    time.sleep(1)
    reset()
    time.sleep(1)
    close()

def close():
   time.sleep(5)

   pi.stop()
   os.system('sudo killall pigpiod')


################################################################################
# Start test and logger
################################################################################

# Test Start Time
TEST_START_TIME = time.time()

# Logger set format
LOG_FORMAT = "%(timegap)s %(levelname)s -- %(message)s"
logging.basicConfig(format=LOG_FORMAT)

# Logger add formatter
logger = logging.getLogger('I2C Master Rx Boot Test')
logger.setLevel('INFO')


# END

################################################################################
# Test Case Module
################################################################################


def main():
    """ Set up for Raspberry Pi configuration """
    global pi
    global MESSAGE_CONFIRMATION
    global dummy

    runTest = ""
    with open(CONFIG_FILE, 'r') as config_toml:
        config = toml.load(config_toml)
        runTest = config['env']['i2c_on_boot']

    if runTest == 'yes':
        logger.info('Initiating I2C Master Rx Boot Test...',
            extra={'timegap': time_gap(TEST_START_TIME)})

        dummy_transaction() # Initiate the dummy transaction to update buffer in proper time
        dummy = True # Update to initiate the dummy transaction properly on the i2c function

        # Set up pi gpio daemon again to clear buffer and reset transaction
        time.sleep(1)
        os.system('sudo pigpiod')
        time.sleep(1)

        pi = pigpio.pi()
        ###################################################################

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

        time.sleep(4)          # Time to wait for messages to be sent (Should see four messages logged)

        MESSAGE_CONFIRMATION = str(MESSAGE_CONFIRMATION)
        MESSAGE_CONFIRMATION = MESSAGE_CONFIRMATION.strip()

        if (MESSAGE_CONFIRMATION == MESSAGE_SENT):

            # Close setup
            close()

            logger.info('Message Sent: ' + MESSAGE_SENT,
                extra={'timegap': time_gap(TEST_START_TIME)})

            logger.info('Message Called Back from Master: ' + MESSAGE_CONFIRMATION,
                extra={'timegap': time_gap(TEST_START_TIME)})

            reset()         # Reset application to stop sending messages

            time.sleep(1)

            logger.info('Connection Satisfied. (Not Expected - But Fine to continue)',
                extra={'timegap': time_gap(TEST_START_TIME)})

            received = True

            time.sleep(1)

            logger.info('I2C Master Rx Test has ended.',
                extra={'timegap': time_gap(TEST_START_TIME)})

        else:

            # Close setup

            close()

            logger.info('Message Sent: ' + MESSAGE_SENT,
                extra={'timegap': time_gap(TEST_START_TIME)})

            logger.info('Message Called Back from Master: ' + MESSAGE_CONFIRMATION,
                extra={'timegap': time_gap(TEST_START_TIME)})

            reset()         # Reset application to stop sending messages

            time.sleep(1)

            logger.info('Connection was not Satisfied. Wrong/No message was sent. (Expected)',
                extra={'timegap': time_gap(TEST_START_TIME)})

            time.sleep(1)

            logger.info('I2C Master Rx Test has ended.',
                extra={'timegap': time_gap(TEST_START_TIME)})
    else:
        logger.info("Not Running I2C Boot Test...",
            extra={'timegap': time_gap(TEST_START_TIME)})
        
        #Closing App
        close()
        reset()
# END

################################################################################
# Test Case Setup
################################################################################

def startup():
        
    time.sleep(1)
    reset()                 # Used to activate the reset button on board
    time.sleep(1)

    logger.info('Setting up for nrf52840dk I2C Master Rx test...',
        extra={'timegap': time_gap(TEST_START_TIME)})

    time.sleep(1)

    main()
            

# END

################################################################################
# Main
################################################################################

if __name__ == '__main__':
    main()
