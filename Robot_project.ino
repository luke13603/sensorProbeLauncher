#include "arduino.h"
#include "startUp.h"
#include "controlSwitch.h"
#include "turrets.h"

extern bool startLight, sequenceActive;

void setup(void) {
  Serial.begin(9600); 
  startSetup();
  turretSetup();
  encoderInit();
}

void loop(void) { 
  buttonCheck();
  startLight = true;
  delay(50);
  if(startLight == true){
    Serial.println("TOGGLE");
    delay(500);
    clutchLoop(); //run the FSM
    setState(LOCKING); //resets the FSM
    sequenceActive = true; //resets the FSM 
    startLight = false; //reset button
  }
}
