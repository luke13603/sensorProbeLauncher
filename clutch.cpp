#include "Servo.h"
#include "Arduino.h"

#define limitSwitch A2
Servo rightSG, leftSG;
const int relayPin = 11;

void clutchSetup(){
  rightSG.attach(13);
  leftSG.attach(12);
  
  pinMode(relayPin, OUTPUT);
  pinMode(limitSwitch, OUTPUT);

  digitalWrite(relayPin, LOW);
  digitalWrite(limitSwitch, HIGH);

  rightSG.write(180);
  leftSG.write(0);
}

void clutchIn(){
  rightSG.write(160);
  leftSG.write(20);
}

void clutchOut(){
  rightSG.write(180);
  leftSG.write(0);
}

void reelIn(){
  while(digitalRead(limitSwitch) == HIGH){
    digitalWrite(relayPin, HIGH);
  }
  digitalWrite(relayPin, LOW);
}