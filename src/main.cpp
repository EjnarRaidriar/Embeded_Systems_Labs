#include <Arduino.h>

#include "lab_1/lab_1.hpp"

// #define APP_NAME LAB_0
#define APP_NAME LAB_1

void setup() {
  #if APP_NAME == LAB_1
    lab_1_setup();
  #endif
}

void loop() {
  #if APP_NAME == LAB_1
    lab_1_loop();
  #endif
}