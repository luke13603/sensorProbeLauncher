#include "arduino.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// ====== Pin Definitions ======
#define IN1Y 9
#define IN2Y 8
#define nSLEEPY 7

#define relay1 6
#define relay2 5

// ====== Global Variables ======
const int MPU_addr = 0x68;
int16_t aX, aY, aZ, temp, gyro_X, gyro_Y, gyro_Z;
double pitchAngle, yawAngle;

enum Direction { STOP, LEFT, RIGHT, UP, DOWN };

// ====== Setup Functions ======
void turretSetup() {
  int pins[] = {IN1Y, IN2Y, nSLEEPY, relay1, relay2};
  for (int p : pins) pinMode(p, OUTPUT);

  digitalWrite(nSLEEPY, HIGH);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
}

void setupMPU() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

// ====== Motor Control ======
void moveYaw(Direction dir) {
  switch (dir) {
    case LEFT:
      analogWrite(IN1Y, 255);
      digitalWrite(IN2Y, LOW);
      break;
    case RIGHT:
      analogWrite(IN2Y, 255);
      digitalWrite(IN1Y, LOW);
      break;
    default:
      digitalWrite(IN1Y, LOW);
      digitalWrite(IN2Y, LOW);
      break;
  }
}

void movePitch(Direction dir) {
  switch (dir) {
    case UP:
      digitalWrite(relay1, LOW);  // activate relay1
      digitalWrite(relay2, HIGH); // ensure relay2 off
      break;
    case DOWN:
      digitalWrite(relay1, HIGH); // ensure relay1 off
      digitalWrite(relay2, LOW);  // activate relay2
      break;
    case STOP:
      digitalWrite(relay1, HIGH); // both off
      digitalWrite(relay2, HIGH);
      break;
  }
}

// ====== MPU Reading ======
void readMPU() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);

  aX = Wire.read() << 8 | Wire.read();
  aY = Wire.read() << 8 | Wire.read();
  aZ = Wire.read() << 8 | Wire.read();
  temp = Wire.read() << 8 | Wire.read();
  gyro_X = Wire.read() << 8 | Wire.read();
  gyro_Y = Wire.read() << 8 | Wire.read();
  gyro_Z = Wire.read() << 8 | Wire.read();

  float ax = aX / 16384.0;
  float ay = aY / 16384.0;
  float az = aZ / 16384.0;

  pitchAngle = atan2(ay, sqrt(ax * ax + az * az)) * 180 / PI;
  yawAngle = atan2(-ax, az) * 180 / PI;
  Serial.println(pitchAngle);
}

double getYaw() { readMPU(); return yawAngle; }
double getPitch() { readMPU(); return pitchAngle; }
