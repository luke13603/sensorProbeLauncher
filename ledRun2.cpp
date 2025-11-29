#include "arduino.h"

constexpr uint8_t P[] = {10,11,12,13}; //A,B,C,D
constexpr uint8_t ledPin = 9, led_Num = 12;
uint8_t led_index = 0;
constexpr uint8_t LEDS[12][2] = {
  {1,0},{0,1},{2,1},{1,2},{3,2},{2,3},
  {2,0},{0,2},{3,1},{1,3},{3,0},{0,3}};

void setupWatchdog(){pinMode(ledPin, OUTPUT);}
void watchdogOn(){digitalWrite(ledPin, HIGH);}
void watchdogOff(){digitalWrite(ledPin, LOW);}
void ledReset() {for(uint8_t p : P) pinMode(p, INPUT);}

inline void ledOn(uint8_t i) {
  ledReset();
  uint8_t low  = LEDS[i][0]; uint8_t high = LEDS[i][1];
  pinMode(P[low], OUTPUT); pinMode(P[high], OUTPUT);
  digitalWrite(P[low], LOW); digitalWrite(P[high], HIGH);
}

void loopLed() {
    ledOn(led_index); delay(25);
    led_index = (led_index + 1) % led_Num; //runs leds; wraps back around when done
}
