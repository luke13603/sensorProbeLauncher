#include "arduino.h"
#include "ledRun2.h"
#include "ledRing.h"
#define BUTTON_PIN 7
#define limit 6

bool state{false}, firing{false}, endseq{true}, reeling{false}, fuckyou{false}, rainboW{false};

void setup() {
  setupWatchdog();
  setupRing();
  Serial.begin(9600);  // Receive data from Arduino A
  Serial.println("Ready to receive...");
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(limit, INPUT_PULLUP);
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n'); cmd.trim();
    if(cmd == "DONE"){endseq = true;}
    else if(cmd == "reeling"){reeling = !reeling; Serial.println("WHAT");}
    else if(cmd == "ledOn"){state = !state;}
    else if(cmd == "fire"){firing = !firing;}
    else if(cmd == "mode1"){fuckyou = !fuckyou; Serial.println("ME");}
    else if(cmd == "rainbow"){rainboW = !rainboW;}
  }
  while(endseq){
    byte buttonState = digitalRead(BUTTON_PIN);
    if(buttonState == LOW){
      Serial.print("done");endseq = false;
      break;
    }
    ledReset();
    watchdogOn(); delay(750);
    watchdogOff(); delay(250);
  }
  watchdogOff();
  if(reeling){
    ledReset();
    byte limitState = digitalRead(limit);
    if(limitState == LOW){
      Serial.println("STOPP");
    }
  }
  if(rainboW){rainbow(10);}
  else if(state){loopRing();}
  else if(firing){setRingGreen();}
  else if(fuckyou){setRingBlue();Serial.println("SHIT");}
  else{resetRing();}
}
