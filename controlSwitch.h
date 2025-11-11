 #include <Arduino.h>

/*TODO: fix the re-state issue here in a way that doesn't break the entire program
 * when i get rid of the enum in the .h file it breaks the FSM for some reason
 * even though I have the enum declared in the .cpp file
 * no fucking clue as to why this breaks the program
 */
enum ControlSwitch{
  LOCKING,
  PULLING,
  HOLDING,
  YAWTURN,
  PITCHTURN,
  FIRING,
  YAWRETURN,
  PITCHRETURN,
  DONE
};

void clutchSetup();
void clutchLoop();
void setState(ControlSwitch newState);
