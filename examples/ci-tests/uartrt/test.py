import serial
import time
import random
import struct

TARGET_ACKNOWLEDGEMENT = ""

sp = serial.Serial(port="/dev/ttyACM0", baudrate=115200, bytesize=8, timeout=2)
ser = serial.Serial(port="/dev/ttyS0", baudrate=115200, bytesize=8, parity="N", stopbits=1);
print("Starting Uart Rx/TX Test...\n")
while(1):
    c = random.randint(65, 90)
    message = chr(c)
    ser.write(struct.pack('<H', c))

    
    time.sleep(5)
    if(sp.in_waiting > 0):
        # print("A")
        print("Message sent: " + message)
        #time.sleep(5)
        sp.readline()
        if(sp.in_waiting > 0):
            message_received = sp.readline()
            message_received = message_received.decode("Ascii")
            print("Message: " + message_received)
            char_received = message_received[-1]
            print("Echoed: " + char_received + "\n")
            if(char_received == message):
                print("Correct Serial Communication Message Received")
                break
       

print("Uart Rx/Tx Test Passes")
sp.close()
ser.close()
sp.open()
ser.open()
time.sleep(4)
ser.write(b"yeah")


class Nrf52840Test(UartTest):
    def setUp(self):
        logger.info('Setting up for nrf52840dk Uart Rx/Tx test...',
            extra={'timegap': time_gap(TEST_START_TIME)})
