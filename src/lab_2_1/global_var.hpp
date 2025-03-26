#ifndef GLOBAL_VAR_HPP
#define GLOBAL_VAR_HPP

#include <Arduino.h>

#define BUTTON_PIN 2
#define LED_1_PIN 3
#define LED_2_PIN 5
#define INC_BUTTON_PIN 8
#define DEC_BUTTON_PIN 9

#define BASE_REC 50
#define MAX 20

extern byte led_1_state;

extern byte led_2_state;

extern int recurrence;

#endif