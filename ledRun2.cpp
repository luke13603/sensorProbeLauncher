#define A 10
#define B 11
#define C 12
#define D 13

const int ledPin = 9;   

#include "arduino.h"

int led_index = 0;
uint8_t led_Num = 12;

//define which 2 pins control eahc led and which directions current flows
//(x/y) where x is low (cathode) and y is high (anode)
//each pin coresponds to the coords
byte LEDS[][2] = {
  {B, A},
  {A, B},
  {C, B},
  {B, C},
  {D, C},
  {C, D},
  {C, A},
  {A, C},
  {D, B},
  {B, D},
  {D, A},
  {A, D},
};

void setupWatchdog(){
  pinMode(ledPin, OUTPUT);
}

void watchdogOn(){
  digitalWrite(ledPin, HIGH);
}

void watchdogOff(){
  digitalWrite(ledPin, LOW);
}
void ledReset() {
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(C, INPUT);
  pinMode(D, INPUT);
}

void ledOff() {
  ledReset();
}

void ledOn(int index) {
  ledReset(); //avoid ghosting
  pinMode(LEDS[index][0], OUTPUT);
  pinMode(LEDS[index][1], OUTPUT);
  digitalWrite(LEDS[index][0], LOW);
  digitalWrite(LEDS[index][1], HIGH);
}

void loopLed() {
    ledOn(led_index);
    //delayMicroseconds(500);//All On
    delay(25);
    led_index = (led_index + 1) % led_Num; //runs through the leds and wraps back around when done
}
