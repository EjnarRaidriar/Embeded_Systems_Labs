#include <Arduino.h>

#include "lab/lab.hpp"
#include "lab0/lab_0.hpp"
#include "lab_1/lab_1.hpp"
#include "lab_1-2/lab_1_2.hpp"
#include "lab_2_1/lab_2_1.hpp"

#define LAB -1
#define LAB_0 0
#define LAB_1 1
#define LAB_1_2 12
#define LAB_2_1 21

#define APP_NAME LAB_2_1

void setup() {
  #if APP_NAME == LAB
    lab_setup();
  #elif APP_NAME == LAB_0
    lab_0_setup();
  #elif APP_NAME == LAB_1
    lab_1_setup();
  #elif APP_NAME == LAB_1_2
    lab_1_2_setup();
  #elif APP_NAME == LAB_2_1
    lab_2_1_setup();
  #endif
}

void loop() {
  #if APP_NAME == LAB
    lab_loop();
  #elif APP_NAME == LAB_0
    lab_0_loop();
  #elif APP_NAME == LAB_1
    lab_1_loop();
  #elif APP_NAME == LAB_1_2
    lab_1_2_loop();
  #elif APP_NAME == LAB_2_1
    lab_2_1_loop();
  #endif
}