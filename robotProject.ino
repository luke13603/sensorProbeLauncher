#include "finiteStateMachine.h"
#include "Arduino.h"
#include "clutch.h"
#include "turrets.h"

#define startButton A1
bool start = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(startButton, OUTPUT);
  digitalWrite(startButton, HIGH);

  clutchSetup();
  setupAll();
}

void loop() {
  Serial.print("startLight");
  while(!start){
    if(digitalRead(startButton) == LOW){
      start = true;
      Serial.print("startLight");
    }
    runFSM();
  }
}