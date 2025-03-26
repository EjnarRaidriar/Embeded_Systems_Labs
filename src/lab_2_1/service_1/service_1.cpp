#include "service_1.hpp"
#include "../global_var.hpp"

byte button_state = LOW;
byte last_button_state = LOW;

void toggle_led() {
  if (button_state == LOW) {
    if (led_1_state == HIGH) {
      led_1_state = LOW;
    } else {
      led_1_state = HIGH;
    }
  }
  digitalWrite(LED_1_PIN, led_1_state);
}

void service_1_setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_1_PIN, OUTPUT);
  last_button_state = digitalRead(BUTTON_PIN);
}

void service_1_loop() {
  button_state = digitalRead(BUTTON_PIN);
  if (button_state != last_button_state) {
    last_button_state = button_state;
    toggle_led();
  }
}