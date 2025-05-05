#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include <Arduino.h>

struct JoystickPosition {
  int x;
  int y;
};

void joystick_setup(const int &switch_pin);
int get_x_pos(const uint8_t &x_pin);
int get_y_pos(const uint8_t &y_pin);
JoystickPosition get_pos(const uint8_t &x_pin, const uint8_t &y_pin);
int is_pressed(const int &switch_pin);

#endif