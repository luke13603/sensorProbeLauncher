#include "arduino.h"
#include <Servo.h>

const int buttonPin = A1;
const int servo1_Pin=13, servo2_Pin=12;
const int IN1=11, IN2=10;
const int clutchIn = 180;
const int clutchOut = 0;

Servo s1, s2;

int buttonState = 0;
bool startLight = false;

void startSetup(){
  pinMode(buttonPin, OUTPUT);
  digitalWrite(buttonPin, HIGH);
  s1.attach(servo1_Pin); s2.attach(servo2_Pin);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  s1.write(0); s2.write(180);
}

void buttonCheck(){
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW){startLight = true;}
}


//Reel Motor Methods
void reelIn(){digitalWrite(IN1, 1); digitalWrite(IN2, 0);}
void reelBrake(){digitalWrite(IN1, 1); digitalWrite(IN2, 1);}
void reelCoast(){digitalWrite(IN1, 0); digitalWrite(IN2, 0);}

//Servo Motor Methods
void clutchGrab(){s1.write(45); s2.write(135);}
void clutchRelease(){s1.write(0); s2.write(180);}
//servos reversed because they are on opposing sides
