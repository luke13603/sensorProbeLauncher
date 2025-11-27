enum motorMove{UP, DOWN, LEFT, RIGHT, BRAKEP, BRAKEY, COASTP, COASTY};

// Setup
void turretSetup();
void encoderUpdatePITCH();
void encoderUpdateYAW();
void encoderInit();

//movement functions
float getPitchAngle();
long getPitchCount();
void resetPitchAngle();

float getYAWangle();
long getYAWCount();
void resetYAWAngle();

void moveTo(motorMove dir, double angle, int speed);
