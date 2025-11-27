#include "arduino.h"
#include "turrets.h"
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <ezButton.h>

//pin defs
const int IN1Y = 8;
const int IN2Y = 7;
const int IN1P = 9;  // Controls speed
const int IN2P = 6;  // Controls direction

#define CLK_PIN_PITCH 5  
#define DT_PIN_PITCH_PITCH  4   
#define DT_PIN_YAW 3
#define CLK_PIN_YAW 2

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

static int counterPITCH = 0;
static int direction = DIRECTION_CW;
static int prev_CLK_state_PITCH = 0;

static int counterYAW = 0;
static int prev_CLK_stateYAW = 0;

//setup for turret
void turretSetup() {
  int pins[] = {IN1Y, IN2Y, IN1P, IN2P};
  for (int p : pins) pinMode(p, OUTPUT);
  digitalWrite(IN2Y, LOW);
  digitalWrite(IN1Y, LOW);
  digitalWrite(IN1P, LOW);
  digitalWrite(IN2P, LOW);
}

void encoderInit() {
    pinMode(CLK_PIN_PITCH, INPUT);
    pinMode(DT_PIN_PITCH_PITCH, INPUT);
    prev_CLK_state_PITCH = digitalRead(CLK_PIN_PITCH);
    resetPitchAngle();

    pinMode(CLK_PIN_YAW, INPUT);
    pinMode(DT_PIN_YAW, INPUT);
    prev_CLK_stateYAW = digitalRead(CLK_PIN_YAW);
}

void encoderUpdateYAW() {
    int CLK_state_YAW = digitalRead(CLK_PIN_YAW);
    // capture rising edge
    if (CLK_state_YAW != prev_CLK_stateYAW && CLK_state_YAW == HIGH) {
        if (digitalRead(DT_PIN_YAW) == HIGH) {counterYAW--; direction = DIRECTION_CCW;}
        else {counterYAW++; direction = DIRECTION_CW;}
    }
    prev_CLK_stateYAW = CLK_state_YAW;
}

void encoderUpdatePITCH() {
    int CLK_statePITCH = digitalRead(CLK_PIN_PITCH);
    if (CLK_statePITCH != prev_CLK_state_PITCH && CLK_statePITCH == HIGH) {
        if (digitalRead(DT_PIN_PITCH_PITCH) == HIGH) {counterPITCH--;direction = DIRECTION_CCW;}
        else {counterPITCH++;direction = DIRECTION_CW;}
    }
    prev_CLK_state_PITCH = CLK_statePITCH;
}

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
void resetPitchAngle() {counterPITCH = 0;}

long getYAWCount() {return counterYAW;}
void resetYAWAngle() {counterYAW = 0;}

void moveTo(motorMove dir, double angle, int speed){
  switch(dir){
    case RIGHT:
      //right
      while (true) {
        encoderUpdateYAW();
        float yaw = getYAWAngle();
        Serial.println(yaw);
        if (yaw >= angle) {
           Serial.println(F("✅ Done, target achieved"));
           digitalWrite(IN1Y, LOW);
           digitalWrite(IN2Y, LOW);
           break;
        }
        else{
          digitalWrite(IN1Y,LOW);
          digitalWrite(IN2Y, HIGH);
        }
      }
      break;
    case LEFT:
      //left
      while (true) {
        encoderUpdateYAW();
        float yaw = getYAWAngle();
        Serial.println(yaw);
        if (yaw >= angle) {
           Serial.println(F("✅ Done, target achieved"));
           digitalWrite(IN1Y, LOW);
           digitalWrite(IN2Y, LOW);
           break;
        }
        else{
          digitalWrite(IN1Y, HIGH);
          digitalWrite(IN2Y, LOW);
        }
      }
      break;
    case UP:
      while (true) {
        encoderUpdatePITCH();
        float pitch = getPitchAngle();
        Serial.println(pitch);
        if (pitch >= angle) {
           Serial.println(F("✅ Done, target achieved"));
           digitalWrite(IN1P, HIGH);
           digitalWrite(IN2P, HIGH);
           break;
        }
        else{
          Serial.println("UP");
          digitalWrite(IN1P, LOW);
          analogWrite(IN2P, speed);
        }
      }
      break;
    case DOWN:
      while (true) {
        encoderUpdatePITCH();
        float pitch = getPitchAngle();
        Serial.println(pitch);
        if (pitch <= angle) {
           Serial.println(F("✅ Done, target achieved"));
           digitalWrite(IN1P, HIGH);
           digitalWrite(IN2P, HIGH);
           break;
        }
        else{
          digitalWrite(IN1P, HIGH);
          analogWrite(IN2P, speed);
        }
      }
      break;
    case BRAKEP:
      digitalWrite(IN1P, HIGH);
      digitalWrite(IN2P, HIGH);
      break;
    case BRAKEY:
      digitalWrite(IN1Y, LOW);
      digitalWrite(IN2Y, LOW);
      break;
    case COASTP:
      digitalWrite(IN1P, LOW);
      digitalWrite(IN2P, LOW);
      break;
    case COASTY:
      digitalWrite(IN2Y, HIGH);
      digitalWrite(IN1Y, HIGH);
      break;
    default:
      digitalWrite(IN2Y, LOW);
      digitalWrite(IN1Y, LOW);
      digitalWrite(IN1P, LOW);
      digitalWrite(IN2P, LOW);
  }
}
