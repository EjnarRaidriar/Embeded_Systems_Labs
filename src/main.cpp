#include "config.h"

#include <Arduino.h>

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
#elif APP_NAME == LAB_3_2
  #include "lab_3_2/lab_3_2.hpp"
  #define SETUP() lab_3_2_setup()
  #define LOOP() lab_3_2_loop()
#elif APP_NAME == LAB_4_1
  #include "lab_4_1/lab_4_1.hpp"
  #define SETUP() lab_4_1_setup()
  #define LOOP() lab_4_1_loop()
#elif APP_NAME == LAB_4_2
  #include "lab_4_2/lab_4_2.hpp"
  #define SETUP() lab_4_2_setup()
  #define LOOP() lab_4_2_loop()
#elif APP_NAME == LAB_5_1
  #include "lab_5_1/lab_5_1.hpp"
  #define SETUP() lab_5_1_setup()
  #define LOOP() lab_5_1_loop()
#elif APP_NAME == LAB_5_2
  #include "lab_5_2/lab_5_2.hpp"
  #define SETUP() lab_5_2_setup()
  #define LOOP() lab_5_2_loop()
#elif APP_NAME == LAB_6_1
  #include "lab_6_1/lab_6_1.hpp"
  #define SETUP() lab_6_1_setup()
  #define LOOP() lab_6_1_loop()
#elif APP_NAME == LAB_6_2
  #include "lab_6_2/lab_6_2.hpp"
  #define SETUP() lab_6_2_setup()
  #define LOOP() lab_6_2_loop()
#endif




void setup() {
  SETUP();
}

void loop() {
  LOOP();
}
