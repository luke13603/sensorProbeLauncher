#include <Servo.h>
#include "arduino.h"

const int servoOnePin = 13;
const int servoTwoPin = 12;
const int IN1 = 11;  // Connected to DRV8871 IN1
const int IN2 = 10;  // Connected to DRV8871 IN2
const int clutchIn = 180;
const int clutchOut = 0;

Servo servoOne;
Servo servoTwo;

void clutchSetup(){
  servoOne.attach(servoOnePin);
  servoTwo.attach(servoTwoPin);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  servoOne.write(0);
  servoTwo.write(180);
}

//drive motor actuates and reels in
void reelIn(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

//drive motor stops moving
void reelBrake(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
}

void reelCoast(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

//both servos push in too activate the clutch
//positions are reversed because they are on opposite sides
void clutchGrab(){
  servoOne.write(65);
  servoTwo.write(115);
}

//same principle as the above method
void clutchRelease(){
  servoOne.write(clutchOut);
  servoTwo.write(clutchIn);
}
