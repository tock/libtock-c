# Temperature controller

This app will display a desired setpoint and allow the user to adjust this value using 
the dev boards push buttons. The global setpoint will also be displayed on the screen.
Upon a change in the desired temperature setpoint, IPC will need to notify the thread 
app to transmit a packet. 

Leon, for now I think it would be best if you just wrote an app that sets up and configures
the push buttons that control a value (increment/decrement). It would also be great if 
you could setup the mechanisms needed for IPC.
