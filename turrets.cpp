#include "headers.h"
const int IN1Y = 8, IN2Y = 7, IN1P = 9, IN2P = 6; //pin defs, 1 is speed, 2 is direction
const int CLK_PITCH = 5, DT_PITCH = 4, DT_YAW = 3, CLK_YAW = 2, DIRECTION_CW = 1, DIRECTION_CCW = -1;  

bool motorForwardPITCH{true}, motorForwardYAW{true};
int MultiplierPITCH = (motorForwardPITCH ? 1 : -1), MultiplierYAW = (motorForwardYAW ? 1 : -1);

//const defs
static const float gearRatioPITCH = 55.0 / 24.0, gearRatioYAW = 55.0 / 12.0, countsPerRev = 30.0;
static const float turretcountsPerRev = countsPerRev * gearRatioPITCH;
static const float turretcountsPerRevYAW = countsPerRev * gearRatioYAW;
static const float degreesPerCountPITCH = 360.0 / turretcountsPerRev;
static const float degreesPerCountYAW = 360.0 / turretcountsPerRevYAW;
static int counterP = 0, prevPitchCLK = 0, counterY = 0, prevYawCLK = 0, direction = DIRECTION_CW;

void turretSetup() {
  int pins[] = {IN1Y, IN2Y, IN1P, IN2P};
  for (int p : pins) pinMode(p, OUTPUT); for (int p : pins) digitalWrite(p, LOW);
}

void encoderInit() {
  int logic[] = {CLK_PITCH, DT_PITCH, CLK_YAW, DT_YAW};
  for (int L : logic) pinMode(L, INPUT);
  prevPitchCLK = digitalRead(CLK_PITCH);
  prevYawCLK = digitalRead(CLK_YAW);
  resetPitchAngle();
}

void encoderUpdate(int CLK_PIN, int DT_PIN, int &prevCLK, int &counter){
  int clk = digitalRead(CLK_PIN);
  if(clk != prevCLK && clk == HIGH){counter += (digitalRead(DT_PIN) == HIGH ? -1 : 1);}
  prevCLK = clk;
}

void encoderUpdatePITCH(){encoderUpdate(CLK_PITCH, DT_PITCH, prevPitchCLK, counterP);}
void encoderUpdateYAW(){encoderUpdate(CLK_YAW, DT_YAW, prevYawCLK, counterY); }

float wrap360(float a){return fmod((fmod(a,360)+360),360);} //forces angle into -360,360 range
float getPitchAngle(){float a=wrap360(counterP*degreesPerCountPITCH);Serial.println(a);return a;}
float getYAWAngle(){float a=wrap360(counterY*degreesPerCountYAW);  Serial.println(a);return a;}

long getPitchCount() {return counterP;}
long getYAWCount() {return counterY;}
void resetPitchAngle() {counterP = 0;}
void resetYAWAngle() {counterY = 0;}

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
