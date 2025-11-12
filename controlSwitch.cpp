#include "Clutch.h"
#include "turrets.h"
#include "arduino.h"

//variables that control the end of each state of the FSM
bool lockingDone{false}, pullingDone{false}, holdingDone{false}, pitchDone{false}, yawDone{false}, 
     firingDone{false}, yawReturnDone{false}, pitchReturnDone{false}, sequenceActive{true};

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

/*HUGGGGE FUCKING ISSUE
 * Motor will not be able to "break" or hold position after the arudino is done calling it to move to pos
 * I COMPLETLY FUCKING FORGOT THAT THE ARDUINO CAN'T RUN ASYNC STUFF
 * FUCK
 * SHIT
 * Fix: move the yaw and pitch turn to the start of the FSM
 * Robot should be able to turn and be done turning before reeling in rubbber bands
 * and then should immedietly go into shooting, thus this wont become a problem.
 * 
 * change the way that the mechansim flips from the state
 */
//Sets the starting position for the FSM
ControlSwitch controlSwitch = LOCKING;

//big ass FSM to control the entire movement sequence of the robot
void clutchLoop() {
  if (!sequenceActive) return;
  switch (controlSwitch) {
    case LOCKING:
      //robot locks the clutch gear onto the reel gear using the servos and gear rack
      Serial.println("TOGGLE");
      delay(500);
      clutchGrab();
      
      while(getPitch() > 30){
        movePitch(UP);
        readMPU();
      }
      movePitch(BRAKE);
      
      lockingDone = true;
      delay(2000);
      //calls to the next state
      if (lockingDone) controlSwitch = PULLING;

    case PULLING:
      //clutch gear starts to reel in the plunger and pull back the rubber bands
      reelIn();
      delay(2800);
      pullingDone = true;
      if (pullingDone) controlSwitch = HOLDING;

    case HOLDING:
      //the clutch holds the rubber band reel in place
      //TODO: check to make sure the servos don't fire off as soon as the FSM moves on
      reelBrake();
      delay(4000);
      holdingDone = true;
      if (holdingDone) controlSwitch = YAWTURN;

    case YAWTURN:
      
      delay(3000);
      //have 3 seconds for the yaw gear to turn
      yawDone = true;
      if (yawDone) controlSwitch = PITCHTURN;

    case PITCHTURN:
      
      while(getPitch() < 50){
        movePitch(DOWN);
        readMPU();
      }
      movePitch(BRAKE);
      
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
      clutchRelease();
      delay(2000);
      firingDone = true;
      if(firingDone) controlSwitch = YAWRETURN;
      
    case YAWRETURN:
      Serial.println("BigFire");
      delay(500);
      Serial.println("TOGGLE");
      delay(500);
      //returns the yaw gear to original position
      delay(3000);
      yawReturnDone = true;
      if(yawReturnDone) controlSwitch = PITCHRETURN;
        
    case PITCHRETURN:
      while(getPitch() < 80){
        movePitch(DOWN);
        readMPU();
      }
      movePitch(COAST);
      
      delay(3000);
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
