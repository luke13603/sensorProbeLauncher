--VERSION 1.0.0--

VERY IMPORTANT:
Make sure that arduino A is providing 5v power to most of the servos and motor drivers.
Arduino B should ONLY be providing 5v power to the watchdog LED giving the current setup.
The external 12v battery should be applying power only to the motor drivers and relays.

Arduino to arduino connection is occuring between the serial ports of the arduinos.
Arduino A should be the sender while arduino B is the reciever. 

The code for the project is split across two arduinos with the code scheme as follows:
  -For arduino A: Robot_project, turrets, clutch, controlSwitch, startUP, and encoder (both the .cpp and .h files)
  -For arduino B: Robot_projectB and led_Run2 (both the .cpp and .h files)
Anything marked with "encoder" is usually dealing with the yaw turret.
Anything marked with "encoder2" is usually dealing with the pitch turret. 



--VERSION 1.5.0--

Both pitch and yaw encoders have been merged into one program (turrets.cpp and turrets.h).
  -Code has been simplified using a rising edge detector for direction.
  -Code has been simplified so that the angles will wrap from 0-360 and not include negatives. 

All headers have been switched over to headers.h so that one call to that can include all supporting files. 
  -This means that startUP and clutch where esentially merged because there is no reason to have a seperate file for startup.
  
Button programming has been moved to arduino B to mitigae problems with shorting out arduino A on analog pins.

Various code cleanup has been done to reduce the amount of code from roughly 600 lines to about 300 lines.
  -Pin definitions are declared using arrays 
  -Variables that use repeat variable types are declare in a single line
  -delays don't get their own line and are instead declared after whatever method has motors moving
  -Similar code lines that revolve around one thing are all merged into one line rather than 2 or 3 lines of code.

