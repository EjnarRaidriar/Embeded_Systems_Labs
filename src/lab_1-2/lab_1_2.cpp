#include "lab_1_2.hpp"

#include "stdio_setups/keypad_stdio/keypad_stdio.hpp"
#include "stdio_setups/lcd_stdio/lcd_stdio.hpp"

// todo hardware architecture uml
#include <Arduino.h>

#define PIN_LEN 5

char pin[PIN_LEN] = {'3', '6', '1', '5'};

bool pinFound;

void lab_1_2_setup() {
  Serial.begin(9600);
  keypadSetup();
  lcdSetup();
  printf("Enter Pin:");
  setCursor(0, 1);
}

void lab_1_2_loop() {
  if (!pinFound) {
    char input[PIN_LEN];
    scanf("%s", input);
    if (strcmp(pin, input) == 0) {
      resetScreen();
      printf("Correct PIN :)");
      setCursor(0, 1);
      printf("Reset to retry");
      pinFound = true;
    } else {
      resetScreen();
      printf("Wrong pin!");
      setCursor(0, 1);
      printf("Retry...");
      setCursor(0, 0);
      delay(3000);
      resetScreen();
      printf("Enter Pin:");
      setCursor(0, 1);
    }
  }
}