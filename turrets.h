// ==== Pin Definitions ====
#define IN1Y 9
#define IN2Y 8
#define nSLEEPY 7

#define RELAY1 6
#define RELAY2 5

// ==== Directions ====
enum Direction { STOP, LEFT, RIGHT, UP, DOWN };

// ==== Function Declarations ====

// Setup
void turretSetup();
void setupMPU();

// MPU Functions
void readMPU();
double getYaw();
double getPitch();

// Movement Functions
void moveYaw(Direction dir);
void movePitch(Direction dir);

// Pitch State Machine
void startPitchMove(double target);
void updatePitchControl();

// (Optional) Yaw State Machine – stub for later expansion
void startYawMove(double target);
void updateYawControl();

bool isPitchIdle();
