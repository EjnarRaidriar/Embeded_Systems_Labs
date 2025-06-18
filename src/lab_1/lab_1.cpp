#include "lab_1.hpp"

#if APP_NAME == LAB_1

#include "stdio_setups/serial_stdio/serail_stdio.hpp"
#include "led/led.hpp"

#include <Arduino.h>

void lab_1_setup() {
  serial_setup();
  led_setup();
}

void lab_1_loop() {
  printf("Commands:\n");
  printf("\tled on - turn on the LED\n");
  printf("\tled off - turn off the LED\n");
  char command[50], option[50];
  scanf("%s %s", command, option);
  printf("\n");
  if (strcmp(command, "led") == 0)
  {
    if (strcmp(option, "on") == 0) {
      led_turn_on();
      printf("Led turned on\n");
    } else if (strcmp(option, "off") == 0) {
      led_turn_off();
      printf("Led turned off\n");
    }
  }
  else
  {
    printf("Unknown command\n");
  }
}

#endif