#include "service_2.hpp"
#include "../global_var.hpp"

unsigned long last_led_change = millis();
unsigned long debounce = BASE_REC / recurrence;

void service_2_setup() {
  pinMode(LED_2_PIN, OUTPUT);
}

void service_2_loop() {
  debounce = BASE_REC / recurrence;
  if (led_1_state == LOW) {
    if (millis() - last_led_change >= debounce) {
      last_led_change = millis();
      if (led_2_state == LOW) {
        led_2_state = HIGH;
      } else {
        led_2_state = LOW;
      }
    }
  } else {
    led_2_state = LOW;
  }
  digitalWrite(LED_2_PIN, led_2_state);
}