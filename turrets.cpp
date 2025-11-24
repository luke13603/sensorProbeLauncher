#include "arduino.h"
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "encoder.h"
#include "turrets.h"
#include <ezButton.h>


// ====== Pin Definitions ======
const int IN1Y = 8;
const int IN2Y = 7;
const int IN1P = 9;  // Controls speed
const int IN2P = 6;  // Controls direction


#define CLK_PIN_PITCH 5  
#define DT_PIN_PITCH_PITCH  4   

#define DIRECTION_CW 1
#define DIRECTION_CCW -1

bool motorForward2 = true;
int Multiplier2 = (motorForward2 ? 1 : -1);

static const float gearRatioPITCH = 55.0 / 24.0;
static const float countsPerRevPITCH = 30.0;
static const float turretcountsPerRevPITCH = countsPerRevPITCH * gearRatioPITCH;
static const float degreesPerCount = 360.0 / turretcountsPerRevPITCH;

static int counter = 0;
static int direction = DIRECTION_CW;
static int prev_CLK_state = 0;

// ====== Setup Functions ======
void turretSetup() {
  int pins[] = {IN1Y, IN2Y, IN1P, IN2P};
  for (int p : pins) pinMode(p, OUTPUT);
  digitalWrite(IN2Y, LOW);
  digitalWrite(IN1Y, LOW);
  digitalWrite(IN1P, LOW);
  digitalWrite(IN2P, LOW);
}

void encoderInit2() {
    pinMode(CLK_PIN_PITCH, INPUT);
    pinMode(DT_PIN_PITCH_PITCH, INPUT);
    prev_CLK_state = digitalRead(CLK_PIN_PITCH);
    resetPitchAngle();
}

void encoderUpdate2() {
    int CLK_state = digitalRead(CLK_PIN_PITCH);

    // capture rising edge
    if (CLK_state != prev_CLK_state && CLK_state == HIGH) {

        if (digitalRead(DT_PIN_PITCH_PITCH) == HIGH) {
            counter--;
            direction = DIRECTION_CCW;
        } else {
            counter++;
            direction = DIRECTION_CW;
        }
    }

    prev_CLK_state = CLK_state;
}

float getPitchAngle() {
    float angle = counter * degreesPerCount;
    while (angle >= 360) angle -= 360;
    while (angle < 0) angle += 360;
    Serial.println(angle);
    return angle;
}

long getPitchCount() {
    return counter;
}

void resetPitchAngle() {
    counter = 0;
}

void moveTo(motorMove dir, double angle, int speed){
  switch(dir){
    case RIGHT:
      //right
      while (true) {
        encoderUpdate();
        float yaw = getTurretAngle();
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
        encoderUpdate();
        float yaw = getTurretAngle();
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
        encoderUpdate2();
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
        encoderUpdate2();
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
      digitalWrite(IN2Y, HIGH);
      digitalWrite(IN1Y, HIGH);
      digitalWrite(IN1P, LOW);
      digitalWrite(IN2P, LOW);
  }
}

void test(){
  digitalWrite(IN1Y, HIGH);
  digitalWrite(IN2Y, LOW);
}
