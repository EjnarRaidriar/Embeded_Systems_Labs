#include "service_2.hpp"
#include "../global_var.hpp"

unsigned long last_led_change = millis();
unsigned long debounce = BASE_REC / recurrence;

void service_2_setup() {
  pinMode(LED_2_PIN, OUTPUT);
}

void service_2_loop() {
  if (led_1_state == HIGH) {
    if (millis() - last_led_change >= debounce) {
      if (led_2_state == LOW) {
        led_2_state = HIGH;
      } else {
        led_2_state = LOW;
      }
    }
  } else {
    led_2_state = HIGH;
  }
  digitalWrite(LED_2_PIN, led_2_state);
}