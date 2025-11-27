#include "arduino.h"
#include "ledRun2.h"
#define BUTTON_PIN 7

bool state {false}, firing{false}, endseq{true};

void setup() {
  setupWatchdog();
  Serial.begin(9600);  // Receive data from Arduino A
  Serial.println("Ready to receive...");
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  while(endseq){
    byte buttonState = digitalRead(BUTTON_PIN);
    if(buttonState == LOW){
      Serial.print("done");endseq = false;
      break;
    }
    ledReset();
    watchdogOn();
    delay(750);
    watchdogOff();
    delay(250);
  }
  if(state){loopLed(); watchdogOff();}
  else if(firing){ledReset(); watchdogOn();}
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n'); cmd.trim();
    if (cmd == "TOGGLE") {state = !state;}
    else if(cmd == "BigFire"){firing = !firing;}
    else if(cmd == "DONE"){endseq = true;}
  }
}
