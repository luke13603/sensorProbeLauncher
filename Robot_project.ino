#include "arduino.h"
#include "Clutch.h"
#include "startUp.h"
#include "controlSwitch.h"
#include "turrets.h"

extern bool startLight;
extern bool sequenceActive;

void setup(void) {
  clutchSetup();
  startSetup();
  turretSetup();
  setupMPU();
  Serial.begin(9600); 
}

void loop(void) { 
  buttonCheck();
  if(startLight == true){
    Serial.println("WATCHDOG");
    delay(500);
    clutchLoop(); //run the FSM
    setState(LOCKING); //resets the FSM
    sequenceActive = true; //resets the FSM 
    startLight = false; //reset button
    Serial.println("WATCHDOG");
    delay(500);
  }
}
