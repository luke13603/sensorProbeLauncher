#include "arduino.h"
//This method called to only during the start up of the robot and runs till "run" button is pressed
const int buttonPin = A0;
int buttonState = 0;
bool startLight = false;

void startSetup(){
  pinMode(buttonPin, OUTPUT);
  digitalWrite(buttonPin, HIGH);
}

//I know theres a way to simplify this but I can't remeber how
void buttonCheck(){
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW){
    startLight = true;
    Serial.println("WATCHDOGOFF");
  }
}
