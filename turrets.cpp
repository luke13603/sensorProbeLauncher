#include "arduino.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// ====== Pin Definitions ======
#define IN1Y 8
#define IN2Y 7
const int IN1P = 9;  // Controls speed
const int IN2P = 6;  // Controls direction

int16_t accX_offset = -747;
int16_t accY_offset = -441;
int16_t accZ_offset = -639;

unsigned long lastMicros = 0;

// ====== Global Variables ======
const int MPU_addr = 0x68;
int16_t aX, aY, aZ, temp, gyro_X, gyro_Y, gyro_Z;
float newPitch, newAngle;
double pitchAngle, yawAngle;

// --- Complementary filter variables ---
unsigned long lastTime = 0;
float filteredPitch = 0;
unsigned long lastFilterTime = 0;

enum Direction { BRAKE, COAST, LEFT, RIGHT, UP, DOWN };
//enum motorMove{UP, DOWN, LEFT, RIGHT, BRAKEP, BRAKEY, COASTP, COASTY};

// ====== Setup Functions ======
void turretSetup() {
  int pins[] = {IN1Y, IN2Y, IN1P, IN2P};
  for (int p : pins) pinMode(p, OUTPUT);
}

void setupMPU() {
  Wire.begin();
  Wire.setClock(100000);
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}


// ====== Motor Control ======
void moveYaw(Direction dir) {
  switch (dir) {
    case LEFT:
      digitalWrite(IN1Y,LOW);
      digitalWrite(IN2Y, HIGH);
      break;
    case RIGHT:
      digitalWrite(IN1Y, HIGH);
      digitalWrite(IN2Y, LOW);
      break;
    case COAST:
      digitalWrite(IN2Y, LOW);
      digitalWrite(IN1Y, LOW);
      break;
  }
}

void movePitch(Direction dir) {
  switch (dir) {
    case UP:
      digitalWrite(IN1P, LOW);
      analogWrite(IN2P, 100);
      break;
    case DOWN:
      digitalWrite(IN1P, HIGH);
      analogWrite(IN2P, 100);
      break;
    case BRAKE:
      digitalWrite(IN1P, HIGH);
      digitalWrite(IN2P, HIGH);
      break;
    case COAST:
      digitalWrite(IN1P, LOW);
      digitalWrite(IN2P, LOW);
      break;
  }
}

// ====== MPU Reading ======
float readMPUPitch() {

  // Request data
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  if (Wire.endTransmission(false) != 0) return NAN;

  int bytes = Wire.requestFrom(MPU_addr, 14, true);
  if (bytes != 14) {
    // BAD READ → skip
    return NAN;
  }

  int16_t rawAX = Wire.read()<<8 | Wire.read();
  int16_t rawAY = Wire.read()<<8 | Wire.read();
  int16_t rawAZ = Wire.read()<<8 | Wire.read();

  int16_t rawTemp = Wire.read()<<8 | Wire.read();

  int16_t rawGX = Wire.read()<<8 | Wire.read();
  int16_t rawGY = Wire.read()<<8 | Wire.read();
  int16_t rawGZ = Wire.read()<<8 | Wire.read();

  // Apply offsets
  float ax = (rawAX - accX_offset) / 16384.0;
  float ay = (rawAY - accY_offset) / 16384.0;
  float az = (rawAZ - accZ_offset) / 16384.0;

  // Convert to pitch
  float pitch = atan2(ay, sqrt(ax*ax + az*az)) * -180.0 / PI;
  pitch += 77;   // your calibration

  Serial.println(pitch);
  return pitch;
}

/*void moveTo(motorMove dir, int speed, double angle){
  switch(dir){
    case LEFT:
      
    case RIGHT:

    case UP:
      while (true) {
        float pitch = getStablePitch();
        if (pitch <= angle) {
           Serial.println(F("✅ Done, target achieved"));
           movePitch(BRAKE);
           break;
        }
        else{
          digitalWrite(IN1P, LOW);
          analogWrite(IN2P, speed);
        }
      }
    case DOWN:
      while (true) {
        float pitch = getStablePitch();
        if (pitch >= angle) {
           Serial.println(F("✅ Done, target achieved"));
           movePitch(BRAKE);
           break;
        }
        else{
          digitalWrite(IN1P, HIGH);
          analogWrite(IN2P, speed);
        }
      }
    case BRAKEP:
      digitalWrite(IN1P, HIGH);
      digitalWrite(IN2P, HIGH);
    case BRAKEY:
      digitalWrite(IN1Y, HIGH);
      digitalWrite(IN2Y, HIGH);
    case COASTP:
      digitalWrite(IN1P, LOW);
      digitalWrite(IN2P, LOW);
    case COASTY:
      digitalWrite(IN2Y, LOW);
      digitalWrite(IN1Y, LOW);
  }
}*/
