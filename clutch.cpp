#include "headers.h"
//servo1 (black wire) is front, servo2 is back
constexpr uint8_t servo1_Pin=13, servo2_Pin=12, IN1=11, IN2=10;
Servo servoONE, servoTWO;

void sSetup(){
  servoONE.attach(servo1_Pin); pinMode(IN1, OUTPUT); servoONE.write(0);
  servoTWO.attach(servo2_Pin); pinMode(IN2, OUTPUT); servoTWO.write(180);
}

//Reel Motor Methods
void reelIn(){analogWrite(IN1, 150); digitalWrite(IN2, LOW);}
void reelBrake(){digitalWrite(IN1, 1); digitalWrite(IN2, 1);}
void reelCoast(){digitalWrite(IN1, 0); digitalWrite(IN2, 0);}

//Servo Motor Methods, servos reversed (opposing sides)
void clutchGrab(){servoONE.write(45); servoTWO.write(135);}
void clutchRelease(){servoONE.write(0); servoTWO.write(180);}
