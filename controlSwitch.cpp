#include "headers.h"
//variables that control the end of each state of the FSM
bool lockingDone{false}, pullingDone{false}, holdingDone{false}, pitchDone{false}, yawDone{false}, 
     firingDone{false}, yawReturnDone{false}, pitchReturnDone{false}, sequenceActive{true};
ControlSwitch controlSwitch = LOCKING;
int mode2Count = 0;  // stores the current count value
Joystick js(A4, A3, 19);
bool wow = true;

void clutchLoop() {
  if (!sequenceActive) return;
  switch (controlSwitch) {
    case LOCKING:
      Serial.println("ledOn");
      delay(500);
      clutchGrab();
      moveTo(UP, 70, 110); delay(3000);
      lockingDone = true;
      if (lockingDone) controlSwitch = YAWTURN;

    case YAWTURN:
      moveTo(RIGHT, 45, NULL);
      yawDone = true; lockingDone = false;
      resetYAWAngle();
      if (yawDone) controlSwitch = PULLING;

    case PULLING:
      Serial.println("ledOn");
      delay(2000);
      reelIn();
      Serial.println("reeling");
      while(wow){
        if (Serial.available()) {
          String cmd = Serial.readStringUntil('\n'); cmd.trim();
          if(cmd == "STOPP"){reelCoast(); Serial.println("reeling"); wow = false; break;}
        } 
      } 
      reelBrake();
      pullingDone = true; yawDone = false;
      if (pullingDone) controlSwitch = PITCHTURN;

    case PITCHTURN:
      Serial.println("fire");
      delay(200);
      //if(mode2Count > 15){moveTo(DOWN, mode2Count, 155); delay(1000);}
      moveTo(DOWN, 15, 1); delay(200);
      pitchDone = true; pullingDone = false; 
      if(pitchDone) controlSwitch = FIRING;
      
    case FIRING:
      reelCoast();
      clutchRelease(); delay(1000);
      Serial.println("fire");
      delay(200);
      pitchDone = false;
      firingDone = true;
      Serial.println("mode1");
      delay(500);
      if(firingDone) controlSwitch = YAWRETURN;
      
    case YAWRETURN:
      moveTo(LEFT, 45, NULL);
      resetYAWAngle();
      firingDone = false; yawReturnDone = true;
      if(yawReturnDone) controlSwitch = PITCHRETURN;
        
    case PITCHRETURN:
      moveTo(DOWN, 5, 110); delay(2000);
      yawReturnDone = false; pitchReturnDone = true;
      Serial.println("mode1");
      delay(500); 
      Serial.println("rainbow");
      delay(4000);
      Serial.println("rainbow");
      if (pitchReturnDone) {
        pitchReturnDone = false;
        controlSwitch = DONE; sequenceActive = false; wow = true;
      }
  }
}
void setState(ControlSwitch newState) {controlSwitch = newState;}
