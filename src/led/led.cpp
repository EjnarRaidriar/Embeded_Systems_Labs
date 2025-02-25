#include "led.hpp"

#include <Arduino.h>

void led_setup() {
  pinMode(LED_PIN, OUTPUT);
}

void led_turn_on() {
  digitalWrite(LED_PIN, HIGH);
}

void led_turn_off() {
  digitalWrite(LED_PIN, LOW);
}

int led_read() {
  return digitalRead(LED_PIN);
}

bool led_is_on() {
  return led_read() == HIGH;
}

void led_toggle() {
  if (led_is_on()) {
    led_turn_off();
  } else {
    led_turn_on();
  }
}