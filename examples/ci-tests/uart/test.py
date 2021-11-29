import serial
import time

TARGET_RECEIVED_MESSAGE = "cse145 is cool"
TARGET_ACKNOWLEDGEMENT = "true"

sp = serial.Serial(port="/dev/ttyUSB0", baudrate=115200, bytesize=8, timeout=2)
print("Starting Uart Test...")
while(True):
    # print("B")
    if(sp.in_waiting > 0):
        message_received = sp.readline()
        message_received = message_received.decode("Ascii")
        # print(message_received)
        # print("A")
        
        if(message_received == TARGET_RECEIVED_MESSAGE):
            print("Correct Serial Communication Message Received")
            break

print("Uart Read Test Passes")
sp.close()
sp.open()
time.sleep(5)
sp.write(b"yeah")

    
        
    
