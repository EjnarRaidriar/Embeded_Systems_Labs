#include "led_v2.hpp"

#include <Arduino.h>

void led_v2_setup(const int &led) {
  pinMode(led, OUTPUT);
}

void led_v2_turn_on(const int &led) {
  digitalWrite(led, HIGH);
}

void led_v2_turn_off(const int &led) {
  digitalWrite(led, LOW);
}

int led_v2_read(const int &led) {
  return digitalRead(led);
}

void led_v2_write(const int &led, uint8_t level) {
    digitalWrite(led, level);
}

bool led_v2_is_on(const int &led) {
  return led_v2_read(led) == HIGH;
}

void led_v2_toggle(const int &led) {
  if (led_v2_is_on(led)) {
    led_v2_turn_off(led);
  } else {
    led_v2_turn_on(led);
  }
}
