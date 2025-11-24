-VERSION 1.0.0-

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
