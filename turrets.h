#define MPU_ADDR 0x68

// ==== Direction Enum ====
enum Direction {
  STOP,
  LEFT,
  RIGHT,
  UP,
  DOWN
};

// ==== Function Declarations ====

// ---- Setup Functions ----
void turretSetup();
void setupMotors();
void setupMPU();
void setupAll();

// ---- Motor Control ----
void moveMotor(int in1, int in2, Direction dir);
void moveYaw(Direction dir);
void movePitch(Direction dir);

// ---- Sensor Readings ----
void readMPU();
double getYaw();
double getPitch();

// ---- Motion Control ----
void moveToAngle(double target, double (*getAngle)(), void (*move)(Direction),
                 Direction dirUp, Direction dirDown);
void yawMoveTo(double target);
void pitchMoveTo(double target);