#include <Arduino.h>

#define LAB -1
#define LAB_0 0
#define LAB_1 1
#define LAB_1_2 12
#define LAB_2_1 21
#define LAB_2_2 22
#define LAB_3_1 31

#define APP_NAME LAB_3_1

#if APP_NAME == LAB
  #include "lab/lab.hpp"
  #define SETUP() lab_setup()
  #define LOOP() lab_loop()
#elif APP_NAME == LAB_0
  #include "lab0/lab_0.hpp"
  #define SETUP() lab_0_setup()
  #define LOOP() lab_0_loop()
#elif APP_NAME == LAB_1
  #include "lab_1/lab_1.hpp"
  #define SETUP() lab_1_setup()
  #define LOOP() lab_1_loop()
#elif APP_NAME == LAB_1_2
  #include "lab_1-2/lab_1_2.hpp"
  #define SETUP() lab_1_2_setup()
  #define LOOP() lab_1_2_loop()
#elif APP_NAME == LAB_2_1
  #include "lab_2_1/lab_2_1.hpp"
  #define SETUP() lab_2_1_setup()
  #define LOOP() lab_2_1_loop()
#elif APP_NAME == LAB_2_2
  #include "lab_2_2/lab_2_2.hpp"
  #define SETUP() lab_2_2_setup()
  #define LOOP() lab_2_2_loop()
#elif APP_NAME == LAB_3_1
  #include "lab_3_1/lab_3_1.hpp"
  #define SETUP() lab_3_1_setup()
  #define LOOP() lab_3_1_loop()
#endif




void setup() {
  SETUP();
}

void loop() {
  LOOP();
}