#include <Servo.h>
#include "arduino.h"

const int servoOnePin = 13;
const int servoTwoPin = 12;
const int clutchMotorPin = 11;
const int clutchIn = 180;
const int clutchOut = 0;

Servo servoOne;
Servo servoTwo;

void clutchSetup(){
  servoOne.attach(servoOnePin);
  servoTwo.attach(servoTwoPin);
  pinMode(clutchMotorPin, OUTPUT);
  servoOne.write(0);
  servoTwo.write(180);
}

//drive motor actuates and reels in
void reelIn(){
  digitalWrite(clutchMotorPin, HIGH);
}

//drive motor stops moving
void reelStop(){
  digitalWrite(clutchMotorPin, LOW);
}

//both servos push in too activate the clutch
//positions are reversed because they are on opposite sides
void clutchGrab(){
  //45 and 135 before then 60 and 120
  servoOne.write(65);
  servoTwo.write(115);
}

//same principle as the above method
void clutchRelease(){
  servoOne.write(clutchOut);
  servoTwo.write(clutchIn);
}
