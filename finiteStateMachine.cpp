#include "Arduino.h"
#include "clutch.h"
#include "turrets.h"

enum {
  pitchTurn,
  yawTurn,
  reeling,
  holding,
  firing,
  pitchReturn,
  yawReturn,
  DONE
};

bool pitchTurnD{false}, yawTurnD{false}, reelingD{false}, holdingD{false}, firingD{false}, pitchReturnD{false}, yawReturnD{false};

int currentState = pitchTurn;
bool running = false;   // flag to start sequence

// FSM engine
void runFSM() {
  switch (currentState) {
    case pitchTurn:
      pitchMoveTo(45);
      delay(1000);
      if (!pitchTurnD) {currentState = yawTurn;}

    case yawTurn:
      yawMoveTo(30);
      delay(1000);
      if (!yawTurnD) {currentState = reeling;}

    case reeling:
      clutchIn();
      reelIn();
      delay(1000);
      if (!reelingD) {currentState = holding;}
    
    case holding:
      Serial.print("firing");
      delay(1000);
      if (!holdingD) {currentState = firing;}

    case firing:
      clutchOut();
      delay(1000);
      Serial.prnit("firing");
      Serial.print("waiting");
      if (!firingD) {currentState = pitchReturn;}

    case pitchReturn:
      pitchMoveTo(0);
      delay(1000);
      if (!pitchReturnD) {currentState = yawReturn;}

    case yawReturn:
      yawMoveTo(0);
      delay(1000);
      if (!yawReturnD) {running = false; currentState = DONE;}
  }
}