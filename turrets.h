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

// Movement Functions
void moveYaw(Direction dir);
void movePitch(Direction dir);

void movePitchForCheck(Direction dir, unsigned long runTime, float targetPitch);
float getStablePitch(int samples = 20, int delayMs = 10);
float readMPUPitch();
float readMPUYaw();

float getYaw();

void readGyroTest();

void dumpRegisters();
