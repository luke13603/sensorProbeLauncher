#include "arduino.h"
#include "ledRun2.h"

bool state = false;
bool firing = false;
bool wDog = true;

void setup() {
  setupWatchdog();
  Serial.begin(9600);  // Receive data from Arduino A
  Serial.println("Ready to receive...");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "TOGGLE") {
      state = !state;  // invert the boolean variable
    }
    else if(cmd == "BigFire"){
      firing = !firing;
    }
  }
  if(state){
    loopLed();
   }
  else if(firing){
   }
   else{
    ledOff();
    watchdogOn();
    delay(750);
    watchdogOff();
    delay(250);
   }
}
