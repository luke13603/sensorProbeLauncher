#include "Clutch.h"
#include "turrets.h"
#include "arduino.h"
#include "encoder.h"

//variables that control the end of each state of the FSM
bool lockingDone{false}, pullingDone{false}, holdingDone{false}, pitchDone{false}, yawDone{false}, 
     firingDone{false}, yawReturnDone{false}, pitchReturnDone{false}, sequenceActive{true};

const unsigned long timeOutPitch = 500;  // 3 seconds safety limit

extern bool motorForward;

//States of the FSM
enum ControlSwitch{
  LOCKING,
  PULLING,
  HOLDING,
  YAWTURN,
  PITCHTURN,
  FIRING,
  YAWRETURN,
  PITCHRETURN,
  DONE
};

//Sets the starting position for the FSM
ControlSwitch controlSwitch = LOCKING;

void clutchLoop() {
  if (!sequenceActive) return;
  switch (controlSwitch) {
    case LOCKING:
      //clutchGrab();
      
      //move upward in 500 ms bursts until pitch <= 40°
      while (true) {
        float pitch = readMPUPitch();
        Serial.println(pitch);
        if (pitch >= 57) {
           Serial.println(F("✅ Done, target achieved"));
           movePitch(BRAKE);
           break;
        }
        else{movePitch(UP);}
        delay(10);
      }

      lockingDone = true;
      delay(1000);
      //calls to the next state
      if (lockingDone) controlSwitch = PULLING;

    case PULLING:
      //clutch gear starts to reel in the plunger and pull back the rubber bands
      reelIn();
      Serial.println("pulling");
      delay(1500);
      pullingDone = true;
      if (pullingDone) controlSwitch = HOLDING;

    case HOLDING:
      //the clutch holds the rubber band reel in place
      //TODO: check to make sure the servos don't fire off as soon as the FSM moves on
      reelBrake();
      delay(1000);
      Serial.println("holding");
      holdingDone = true;
      if (holdingDone) controlSwitch = YAWTURN;

    case YAWTURN:
      Serial.println("YawTurn");
      while (true) {
        encoderUpdate();
        float yaw = getTurretAngle();
        Serial.println(yaw);
        if (yaw >= 45) {
           Serial.println(F("✅ Done, target achieved"));
           moveYaw(COAST);
           break;
        }
        else{moveYaw(RIGHT);}
        delay(10);
      }
      yawDone = true;
      resetTurretAngle();
      if (yawDone) controlSwitch = PITCHTURN;

    case PITCHTURN:
      while (true) {
        float pitch = readMPUPitch();
        Serial.println(pitch);
        if (pitch <= 15) {
           Serial.println(F("✅ Done, target achieved"));
           movePitch(BRAKE);
           break;
        }
        else{movePitch(DOWN);}
        delay(10);
      }
      
      delay(3500);
      pitchDone = true;
      Serial.println("TOGGLE");
      delay(500);
      Serial.println("BigFire");
      delay(500);
      if(pitchDone) controlSwitch = FIRING;
      
    case FIRING:
      //the two servos pull the gear rack away from the reel gear
      reelCoast();
      //clutchRelease();
      delay(1000);
      firingDone = true;
      if(firingDone) controlSwitch = YAWRETURN;
      
    case YAWRETURN:
      Serial.println("BigFire");
      delay(500);
      Serial.println("TOGGLE");
      delay(500);
      Serial.println("YawReturn");
      while (true) {
        encoderUpdate();
        float yaw = getTurretAngle();
        Serial.println(yaw);
        if (yaw >= 45) {
           Serial.println(F("✅ Done, target achieved"));
           moveYaw(COAST);
           break;
        }
        else{moveYaw(LEFT);}
        delay(10);
      }
      resetTurretAngle();
      yawReturnDone = true;
      if(yawReturnDone) controlSwitch = PITCHRETURN;
        
    case PITCHRETURN:
      while (true) {
        float pitch = readMPUPitch();
        Serial.println(pitch);
        if (pitch <= 5) {
           Serial.println(F("✅ Done, target achieved"));
           movePitch(COAST);
           break;
        }
        else{movePitch(DOWN);}
        delay(10);
      }
      
      delay(5000);
      pitchReturnDone = true;
      
      //end statement that exits the switch once the pitch return state is done
      if (pitchReturnDone) {
        Serial.println("TOGGLE");
        controlSwitch = DONE;
        sequenceActive = false;
      }
  }
}

void setState(ControlSwitch newState) {
  controlSwitch = newState;
}
