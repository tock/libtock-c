QDEC Callback Test

This test's purposed is to ensure the interrupts work
as expected and the callbacks work as expected every 
time the QDEC is rotated.

To verify the test is working properly, fire up the QDEC 
timer program, and without touching the encoder, you should 
see the following initial statements (If anything else is printed, 
that is a signal that there was an error when setting up the QDEC):

'Starting The [QDEC] Test App for Callbacks
QDEC has been fully initiated!'

Then, turning the QDEC in the positive direction
one click will invoke the callback method and a new statment will
be printed: 

QDEC Absolute Position: 1
QDEC Absolute Position: 2
QDEC Absolute Position: 3
QDEC Absolute Position: 4

Similarly, turning the QDEC in the negative direction (from the initial 
starting location) one click will invoke the callback method and a new 
statment will be printed: 

QDEC Absolute Position: -1
QDEC Absolute Position: -2
QDEC Absolute Position: -3
QDEC Absolute Position: -4

These are good initial tests to ensure the QDEC is working as expected.
Continue the QDEC in both directions and see if the progression of values 
displayed on the screen are logical.
