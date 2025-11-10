#define A 10
#define B 11
#define C 12
#define D 13

#include "arduino.h"

int led_index = 0;
uint8_t led_Num = 12;

byte LEDS[][2] = {
  {B, A}, {A, B}, {C, B}, {B, C},
  {D, C}, {C, D}, {C, A}, {A, C},
  {D, B}, {B, D}, {D, A}, {A, D},
};

void ledReset() {
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(C, INPUT);
  pinMode(D, INPUT);
}

void ledOn(int index) {
  ledReset();
  pinMode(LEDS[index][0], OUTPUT);
  pinMode(LEDS[index][1], OUTPUT);
  digitalWrite(LEDS[index][0], LOW);
  digitalWrite(LEDS[index][1], HIGH);
}

// 🔹 Simulate all LEDs on (fast scan)
void ledAllOn() {
  for (int i = 0; i < led_Num; i++) {
    ledOn(i);
    delayMicroseconds(500); // adjust for brightness vs flicker
  }
  ledReset();
}

void setupLed() {
  ledReset();
}

void loopLed() {
  ledAllOn();  // visually all on
}