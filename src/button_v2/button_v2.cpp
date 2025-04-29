#include "button_v2.hpp"

#include <Arduino.h>

void button_v2_setup(const int& button) {
  pinMode(button, INPUT);
}

int button_v2_read(const int& button) {
  return digitalRead(button);
}

bool button_v2_is_pressed(const int& button) {
  return button_v2_read(button) == HIGH;
}
