// ==== Directions ====
enum motorMove{UP, DOWN, LEFT, RIGHT, BRAKEP, BRAKEY, COASTP, COASTY};

// Setup
void turretSetup();
void encoderUpdate2();
void encoderInit2();

//movement functions
float getPitchAngle();
long getPitchCount();
int getPitchDirection();
void resetPitchAngle();
void moveTo(motorMove dir, double angle, int speed);
void test();
