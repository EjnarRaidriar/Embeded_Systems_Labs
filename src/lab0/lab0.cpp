#include "lab_0.hpp"
#include <Arduino.h>

#define LED 3
#define BUTTON 2

byte lastButtonState;
byte ledState = LOW;

unsigned long lastTimeButtonStateChanged = millis();
unsigned long debounceDuration = 50; // miliseconds

void lab_0_setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
  lastButtonState = digitalRead(BUTTON);
}

void lab_0_loop() {
  if (millis() - lastTimeButtonStateChanged >= debounceDuration) {
    byte buttonState = digitalRead(BUTTON);
    if (buttonState != lastButtonState) {
      lastTimeButtonStateChanged = millis();
      lastButtonState = buttonState;
      if (buttonState == LOW) {
        if (ledState == HIGH) {
          ledState = LOW;
        }
        else {
          ledState = HIGH;
        }
      }
      digitalWrite(LED, ledState);
    }
  }
}