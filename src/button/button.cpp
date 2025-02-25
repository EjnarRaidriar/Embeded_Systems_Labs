#include "button.hpp"

#include <Arduino.h>

void button_setup() {
  pinMode(BUTTON_PIN, INPUT);
}

int button_read() {
  return digitalRead(BUTTON_PIN);
}

bool button_is_pressed() {
  return button_read() == HIGH;
}