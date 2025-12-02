#include "headers.h"

extern Joystick js;
extern bool sequenceActive;
bool huge{false};
unsigned long lastCountTime = 0;  // for timing the slow increase
extern int mode2Count;

void setup(void) {Serial.begin(9600); sSetup(); turretSetup(); encoderInit(); js.begin();}

void loop(void) { 
  js.update();
  if(js.getMode() == 0){
    if (Serial.available()) {
      String cmd = Serial.readStringUntil('\n'); cmd.trim();
    if(cmd == "done"){huge = true;}
    }
    if(huge){
      Serial.println("TOGGLE"); delay(500);
      clutchLoop(); //run the FSM
      setState(LOCKING); sequenceActive = true; //resets the FSM
      huge = false; Serial.print("DONE"); //resets button
    }
  }
  else if(js.getMode() == 1){
    while(true){
      js.update();
      encoderUpdatePITCH();
      Serial.println(getPitchAngle());
      if(js.getMode() != 1){
        stopAll();
        break;
      }
      if(js.getX() > 100){
        moveRight();
      }
      else if(js.getX() < -100){
        moveLeft();
      }
      else if(js.getY() > 100){
        moveUp();
      }
      else if(js.getY() < -100){
        moveDown();
      }
      else{
        stopAll();
      }
    }
  }
  else if (js.getMode() == 2) {
    js.update();  
    int y = js.getY();
    unsigned long now = millis();
    // Only change value every 50 ms (smooth & slow)
    if (now - lastCountTime >= 50) {
      if (y > 100) {  
        // Count upward, max = +90
        if (mode2Count < 90) mode2Count++;
      }
      else if (y < -100) {  
        // Count downward, min = 0
        if (mode2Count > 0) mode2Count--;
      }
      lastCountTime = now;
    }
  // Optional: print to Serial for debugging
  Serial.print("Mode 2 Count = ");
  Serial.println(mode2Count);
  }
}
