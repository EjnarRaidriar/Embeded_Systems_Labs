#include <Arduino.h>

#define LED 13
#define BUTTON 12

byte lastButtonState;
byte ledState = LOW;

unsigned long lastTimeButtonStateChanged = millis();
unsigned long debounceDuration = 50; // miliseconds

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
  lastButtonState = digitalRead(BUTTON);
}

void loop() {
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