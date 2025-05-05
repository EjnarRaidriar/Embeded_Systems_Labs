#include "lab/lab.hpp"

#include <Arduino.h>
#include "stdio_setups/serial_stdio/serail_stdio.hpp"

#define SWITCH 2

void lab_setup() {
  Serial.begin(9600);
  serial_setup();
  pinMode(SWITCH, INPUT_PULLUP);
}

void lab_loop() {
  int x = analogRead(A1);
  int y = analogRead(A0);
  int sStat = digitalRead(SWITCH);
  printf("x: %d; y: %d;\n", x, y);
  if (sStat == LOW) {
    printf("Button is pressed\n");
  }
  delay(500);
}