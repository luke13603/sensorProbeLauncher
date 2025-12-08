#include "arduino.h"
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
    uint8_t pinX, pinY;
    ezButton _button;
    int xValue, yValue, mode = 0; 
};
