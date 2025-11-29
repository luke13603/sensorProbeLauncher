#include "headers.h"
//variables that control the end of each state of the FSM
bool lockingDone{false}, pullingDone{false}, holdingDone{false}, pitchDone{false}, yawDone{false}, 
     firingDone{false}, yawReturnDone{false}, pitchReturnDone{false}, sequenceActive{true};

extern bool motorForward;
ControlSwitch controlSwitch = LOCKING;

void clutchLoop() {
  if (!sequenceActive) return;
  switch (controlSwitch) {
    case LOCKING:
      clutchGrab();
      moveTo(UP, 70, 110); delay(1000);
      lockingDone = true;
      if (lockingDone) controlSwitch = PULLING;

    case PULLING:
      reelIn(); delay(2000);
      pullingDone = true;
      if (pullingDone) controlSwitch = HOLDING;

    case HOLDING:
      reelBrake(); delay(1000);
      holdingDone = true;
      if (holdingDone) controlSwitch = YAWTURN;

    case YAWTURN:
      moveTo(RIGHT, 45, NULL);
      yawDone = true;
      resetYAWAngle();
      if (yawDone) controlSwitch = PITCHTURN;

    case PITCHTURN:
      moveTo(DOWN, 15, 110); delay(2500);
      pitchDone = true;
      Serial.println("TOGGLE"); delay(500);
      Serial.println("BigFire"); delay(500);
      if(pitchDone) controlSwitch = FIRING;
      
    case FIRING:
      reelCoast();
      clutchRelease(); delay(1000);
      firingDone = true;
      if(firingDone) controlSwitch = YAWRETURN;
      
    case YAWRETURN:
      Serial.println("BigFire"); delay(500);
      Serial.println("TOGGLE"); delay(500);
      moveTo(LEFT, 45, NULL);
      resetYAWAngle();
      yawReturnDone = true;
      if(yawReturnDone) controlSwitch = PITCHRETURN;
        
    case PITCHRETURN:
      moveTo(DOWN, 5, 110); delay(2000);
      pitchReturnDone = true;
      if (pitchReturnDone) {
        Serial.println("TOGGLE");
        controlSwitch = DONE; sequenceActive = false;
      }
  }
}

void setState(ControlSwitch newState) {controlSwitch = newState;}
