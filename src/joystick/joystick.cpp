#include "joystick.hpp"

void joystick_setup(const int &switch_pin) {
  pinMode(switch_pin, INPUT_PULLUP);
}

int get_x_pos(const uint8_t &x_pin) {
  return analogRead(x_pin);
}

int get_y_pos(const uint8_t &y_pin) {
  return analogRead(y_pin);
}

JoystickPosition get_pos(const uint8_t &x_pin, const uint8_t &y_pin) {
  return JoystickPosition{analogRead(x_pin), analogRead(y_pin)};
}

int is_pressed(const int &switch_pin) {
  return digitalRead(switch_pin) == LOW;
}