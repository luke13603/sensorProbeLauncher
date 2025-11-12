// ==== Pin Definitions ====
#define IN1Y 7
#define IN2Y 6
#define IN1P 9
#define IN2P 8

// ==== Directions ====
enum Direction { BRAKE, COAST, LEFT, RIGHT, UP, DOWN };

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
