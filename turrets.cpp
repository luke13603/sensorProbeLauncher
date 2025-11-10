#include <Wire.h>
#include "Arduino.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Yaw motor (motor driver)
#define IN1Y 9
#define IN2Y 8
#define nSLEEPY 7

// Pitch motor (2 relays)
#define RELAY_P1 6
#define RELAY_P2 5

// -------------------- Enums --------------------
enum Direction { STOP, LEFT, RIGHT, UP, DOWN };

//MPU6050 global stuff
const int MPU_addr = 0x68; // I2C address for MPU6050
int16_t aX, aY, aZ, temp, gyro_X, gyro_Y, gyro_Z;
double pitchAngle, yawAngle;

//Setup stuff
void setupMotors() {
  // Set pin modes
  int pins[] = {IN1Y, IN2Y, nSLEEPY, RELAY_P1, RELAY_P2};
  for (int p : pins) pinMode(p, OUTPUT);

  // Initialize states
  digitalWrite(nSLEEPY, HIGH);  // Enable yaw driver
  digitalWrite(RELAY_P1, LOW);
  digitalWrite(RELAY_P2, LOW);
}

void setupMPU() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0); // Wake MPU6050
  Wire.endTransmission(true);
}

void turretSetup() {
  setupMotors();
  setupMPU();
  Serial.begin(9600);
  delay(500);
  Serial.println("Turret initialized.");
}

void setupAll(){
  turretSetup();
  setupMPU();
  setupMotors();
}

// ----- Yaw controlled by motor driver -----
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

//Pitch controlled by relays
void movePitch(Direction dir) {
  // Always turn off both relays first
  digitalWrite(RELAY_P1, LOW);
  digitalWrite(RELAY_P2, LOW);
  delay(100); // small delay to protect relay contacts

  switch (dir) {
    case UP:
      digitalWrite(RELAY_P1, HIGH);  // Energize relay 1
      break;
    case DOWN:
      digitalWrite(RELAY_P2, HIGH);  // Energize relay 2
      break;
    default:
      // Both off = STOP
      break;
  }
}

//MPU6050 functions
void readMPU() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // Start at ACCEL_XOUT_H
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
  yawAngle   = atan2(-ax, az) * 180 / PI;

  Serial.print("Pitch = "); Serial.print(pitchAngle);
  Serial.print(" | Yaw = "); Serial.print(yawAngle);
  Serial.print(" | Temp = "); Serial.println(temp / 340.00 + 36.53);
}

double getYaw()   { readMPU(); return yawAngle; }
double getPitch() { readMPU(); return pitchAngle; }

// Motion Control by Angle
void moveToAngle(double target, double (*getAngle)(), void (*move)(Direction),
                 Direction dirUp, Direction dirDown) {
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) {  // 5s timeout safety
    double angle = getAngle();
    if ((dirUp == LEFT || dirUp == UP) ? angle < target : angle > target) {
      move(dirUp);
    } else {
      move(STOP);
      break;
    }
    delay(50); // time for motion + sensor update
  }
  move(STOP);
}

void yawMoveTo(double target) {
  moveToAngle(target, getYaw, moveYaw, LEFT, RIGHT);
}

void pitchMoveTo(double target) {
  moveToAngle(target, getPitch, movePitch, UP, DOWN);
}