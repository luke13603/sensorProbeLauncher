#include "headers.h"
//variables that control the end of each state of the FSM
bool lockingDone{false}, pullingDone{false}, holdingDone{false}, pitchDone{false}, yawDone{false}, 
     firingDone{false}, yawReturnDone{false}, pitchReturnDone{false}, sequenceActive{true};
ControlSwitch controlSwitch = LOCKING;
int mode2Count = 0;  // stores the current count value
Joystick js(A4, A3, 19);

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
      lockingDone = false;
      pullingDone = true;
      if (pullingDone) controlSwitch = HOLDING;

    case HOLDING:
      reelBrake(); delay(1000);
      pullingDone = false;
      holdingDone = true;
      if (holdingDone) controlSwitch = YAWTURN;

    case YAWTURN:
      moveTo(RIGHT, 45, NULL);
      yawDone = true;
      holdingDone = false;
      resetYAWAngle();
      if (yawDone) controlSwitch = PITCHTURN;

    case PITCHTURN:
      if(mode2Count > 15){
        moveTo(DOWN, mode2Count, 155); delay(1000);
      }
      else{
        moveTo(DOWN, 15, 110); delay(2500);
      }
      yawDone = false;
      pitchDone = true;
      Serial.println("TOGGLE"); delay(500);
      Serial.println("BigFire"); delay(500);
      if(pitchDone) controlSwitch = FIRING;
      
    case FIRING:
      reelCoast();
      clutchRelease(); delay(1000);
      pitchDone = false;
      firingDone = true;
      if(firingDone) controlSwitch = YAWRETURN;
      
    case YAWRETURN:
      Serial.println("BigFire"); delay(500);
      Serial.println("TOGGLE"); delay(500);
      moveTo(LEFT, 45, NULL);
      resetYAWAngle();
      firingDone = false;
      yawReturnDone = true;
      if(yawReturnDone) controlSwitch = PITCHRETURN;
        
    case PITCHRETURN:
      moveTo(DOWN, 5, 110); delay(2000);
      yawReturnDone = false;
      pitchReturnDone = true;
      if (pitchReturnDone) {
        pitchReturnDone = false;
        Serial.println("TOGGLE");
        controlSwitch = DONE; sequenceActive = false;
      }
  }
}

void setState(ControlSwitch newState) {controlSwitch = newState;}
