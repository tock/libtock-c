QDEC Timer Test

This test's purposed is to ensure the interrupts work
as expected and the approximately correct displacement
is being returned with each fire of the timer.

To verify the test is working properly, fire up the QDEC 
timer program, and without touching the encoder, you should 
see the statement 'Displacement val: 0' print on the screen
every 500 seconds. Turning the QDEC in the positive direction
one click will change the print statement to 'Displacement 
val: 4' for one timer fired instance and then return back to
'Displacement val: 0'. Similarly, turning it in the negative 
direction one click will change the print statement to 
'Displacement val: -4' for one timer fired instance and then 
return back to 'Displacement val: 0'.
