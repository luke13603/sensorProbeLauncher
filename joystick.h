#include <Arduino.h>
#include "ezButton.h"

class Joystick {
public:
  Joystick(uint8_t pinX, uint8_t pinY, uint8_t pinSW);

  void begin();
  void update();

  int getX();
  int getY();
  bool isPressed();
  bool isReleased();
  int getButtonState();

  int getMode();   // NEW: returns 0,1,2

private:
  uint8_t _pinX;
  uint8_t _pinY;
  ezButton _button;

  int _xValue;
  int _yValue;

  int _mode = 0;   // NEW: mode state
};
