#include "arduino.h"
#include "turrets.h"
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <ezButton.h>

//pin defs, 1 is speed, 2 is direction
const int IN1Y = 8, IN2Y = 7, IN1P = 9, IN2P = 6;

#define CLK_PITCH 5  
#define DT_PITCH  4   
#define DT_YAW 3
#define CLK_YAW 2

#define DIRECTION_CW 1
#define DIRECTION_CCW -1

bool motorForwardPITCH{true}, motorForwardYAW{true};
int MultiplierPITCH = (motorForwardPITCH ? 1 : -1);
int MultiplierYAW = (motorForwardYAW ? 1 : -1);

//const def
static const float gearRatioPITCH = 55.0 / 24.0;
static const float countsPerRev = 30.0;
static const float turretcountsPerRev = countsPerRev * gearRatioPITCH;
static const float degreesPerCountPITCH = 360.0 / turretcountsPerRev;

static const float gearRatioYAW = 55.0 / 12.0;
static const float turretcountsPerRevYAW = countsPerRev * gearRatioYAW;
static const float degreesPerCountYAW = 360.0 / turretcountsPerRevYAW;

static int counterPITCH = 0, prevPitchCLK = 0;
static int counterYAW = 0, prevYawCLK = 0;
static int direction = DIRECTION_CW;

//setup for turret
void turretSetup() {
  int pins[] = {IN1Y, IN2Y, IN1P, IN2P};
  for (int p : pins) pinMode(p, OUTPUT);
  for (int p : pins) digitalWrite(p, LOW);
}

void encoderInit() {
    pinMode(CLK_PITCH, INPUT);
    pinMode(DT_PITCH, INPUT);
    prevPitchCLK = digitalRead(CLK_PITCH);
    resetPitchAngle();

    pinMode(CLK_YAW, INPUT);
    pinMode(DT_YAW, INPUT);
    prevYawCLK = digitalRead(CLK_YAW);
}

void encoderUpdate(int CLK_PIN, int DT_PIN, int &prevCLK, int &counter){
  int clk = digitalRead(CLK_PIN);
  if(clk != prevCLK && clk == HIGH){
    counter += (digitalRead(DT_PIN) == HIGH ? -1 : 1);
  }
  prevCLK = clk;
}

void encoderUpdatePITCH(){encoderUpdate(CLK_PITCH, DT_PITCH, prevPitchCLK, counterPITCH);}
void encoderUpdateYAW(){encoderUpdate(CLK_YAW, DT_YAW, prevYawCLK, counterYAW); }

float getPitchAngle() {
    float angle = counterPITCH * degreesPerCountPITCH;
    while (angle >= 360) angle -= 360;
    while (angle < 0) angle += 360;
    Serial.println(angle);
    return angle;
}

float getYAWAngle() {
    float angle = counterYAW * degreesPerCountYAW;
    // wrap 0–360
    while (angle >= 360) angle -= 360;
    while (angle < 0) angle += 360;
    Serial.println(angle);
    return angle;
}

long getPitchCount() {return counterPITCH;}
long getYAWCount() {return counterYAW;}
void resetPitchAngle() {counterPITCH = 0;}
void resetYAWAngle() {counterYAW = 0;}

void moveTo(motorMove dir, double angle, int speed) {
  bool done = false;

  while (!done) {
    switch (dir) {

      case RIGHT:
        encoderUpdateYAW();
        if (getYAWAngle() >= angle) done = true;
        else digitalWrite(IN1Y,LOW); digitalWrite(IN2Y, HIGH);
        break;

      case LEFT:
        encoderUpdateYAW();
        if (getYAWAngle() >= angle) done = true;
        else { digitalWrite(IN1Y, HIGH); digitalWrite(IN2Y, LOW); }
        break;

      case UP:
        encoderUpdatePITCH();
        if (getPitchAngle() >= angle) done = true;
        else { digitalWrite(IN1P, LOW); analogWrite(IN2P, speed); }
        break;

      case DOWN:
        encoderUpdatePITCH();
        if (getPitchAngle() <= angle) done = true;
        else { digitalWrite(IN1P, HIGH); analogWrite(IN2P, speed); }
        break;

      case BRAKEY: digitalWrite(IN1Y, LOW); digitalWrite(IN2Y, LOW); return;
      case BRAKEP: digitalWrite(IN1P, HIGH); digitalWrite(IN2P, HIGH); return;
      case COASTY: digitalWrite(IN1Y, HIGH); digitalWrite(IN2Y, HIGH); return;
      case COASTP: digitalWrite(IN1P, LOW); digitalWrite(IN2P, LOW); return;
    }
  }

  digitalWrite(IN1Y, LOW); digitalWrite(IN2Y, LOW);
  digitalWrite(IN1P, HIGH); digitalWrite(IN2P, HIGH);
  Serial.println(F("✅ Target reached"));
}
