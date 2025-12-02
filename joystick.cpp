#include "Joystick.h"

Joystick::Joystick(uint8_t pinX, uint8_t pinY, uint8_t pinSW)
  : _pinX(pinX), _pinY(pinY), _button(pinSW) {
}

void Joystick::begin() {
  _button.setDebounceTime(50);
}

void Joystick::update() {
  _button.loop();

  // Handle mode cycling on press
  if (_button.isPressed()) {
    _mode++;
    if (_mode > 2) _mode = 0;
  }

  int rawX = analogRead(_pinX);
  int rawY = analogRead(_pinY);

  // Convert 0–1023 to -255→+255
  _xValue = map(rawX, 0, 1023, -255, 255);
  _yValue = map(rawY, 0, 1023, -255, 255);

  // Flip signs
  _xValue = -_xValue;
  _yValue = -_yValue;
}

int Joystick::getX() {return _xValue;}
int Joystick::getY() {return _yValue;}
bool Joystick::isPressed() {return _button.isPressed();}
bool Joystick::isReleased() {return _button.isReleased();}
int Joystick::getButtonState() {return _button.getState();}
int Joystick::getMode() {return _mode;}
