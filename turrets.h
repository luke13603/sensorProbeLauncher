enum motorMove{UP, DOWN, LEFT, RIGHT, BRAKEP, BRAKEY, COASTP, COASTY};

void turretSetup();
void encoderUpdatePITCH();
void encoderUpdateYAW();
void encoderInit();

float getPitchAngle();
long getPitchCount();
void resetPitchAngle();
long getYAWCount();
void resetYAWAngle();
void moveTo(motorMove dir, double angle, int speed);

void moveUp();
void moveDown();
void moveLeft();
void moveRight();
void stopAll();
