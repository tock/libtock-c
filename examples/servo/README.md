Servo Controller App  
=====================  

A simple app that tests the functionality of a connected servo motor by reading its initial angle, adjusting the angle from 0 to 180 degrees, and verifying the actual angle at each step.

Example Output  
--------------  

```  
The number of available servomotors is: 1  
Requested angle 0. Actual current angle is: 0  
Requested angle 1. Actual current angle is: 1  
Requested angle 2. Actual current angle is: 2  
...  
Requested angle 180. Actual current angle is: 180  
```  

If the servo index is out of range:  
```  
The index number is bigger than the available servomotors  
```  

If the servo cannot report its angle:  
```  
This servo cannot return its angle.  
```  