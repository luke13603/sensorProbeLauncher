#include "arduino.h"
#include "clutch.h"
#include "controlSwitch.h"
#include "turrets.h"

extern bool startLight, sequenceActive;
bool huge{false};

void setup(void) {
  Serial.begin(9600); 
  sSetup();
  turretSetup();
  encoderInit();
}

void loop(void) { 
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n'); cmd.trim();
    if(cmd == "done"){huge = true;}
  }
  if(huge){
    Serial.println("TOGGLE");
    delay(500);
    clutchLoop(); //run the FSM
    setState(LOCKING); sequenceActive = true; //resets the FSM
    startLight = false; //reset button
    huge = false;
    Serial.print("DONE");
  }
}
