#include "arduino.h"
#include "Clutch.h"
#include "startUp.h"
#include "controlSwitch.h"
#include "turrets.h"
#include "encoder.h"

extern bool startLight;
extern bool sequenceActive;

void setup(void) {
  Serial.begin(9600); 
  clutchSetup();
  startSetup();
  turretSetup();
  setupMPU();
  encoderInit();
}

void loop(void) { 
  buttonCheck();
  if(startLight == true){
    Serial.println("TOGGLE");
    delay(500);
    clutchLoop(); //run the FSM
    setState(LOCKING); //resets the FSM
    sequenceActive = true; //resets the FSM 
    startLight = false; //reset button
  }
}
