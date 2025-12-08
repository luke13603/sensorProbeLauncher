#include "joystick.h"

Joystick::Joystick(uint8_t pinX, uint8_t pinY, uint8_t pinSW)
  : pinX(pinX), pinY(pinY), _button(pinSW) {
}

void Joystick::begin() {_button.setDebounceTime(50);}

void Joystick::update() {
  _button.loop();

  // Handle mode cycling on press
  if (_button.isPressed()) {mode++; if (mode > 2) mode = 0;}
  int rawX = analogRead(pinX), rawY = analogRead(pinY);

  // Convert 0–1023 to -255→+255
  xValue = -map(rawX, 0, 1023, -255, 255);
  yValue = -map(rawY, 0, 1023, -255, 255);
}

int Joystick::getX() {return xValue;}
int Joystick::getY() {return yValue;}
bool Joystick::isPressed() {return _button.isPressed();}
bool Joystick::isReleased() {return _button.isReleased();}
int Joystick::getButtonState() {return _button.getState();}
int Joystick::getMode() {return mode;}
