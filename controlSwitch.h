#include <Arduino.h>
enum ControlSwitch{LOCKING, PULLING, HOLDING, YAWTURN, PITCHTURN,FIRING, YAWRETURN, PITCHRETURN, DONE};
void clutchLoop();
void setState(ControlSwitch newState);
