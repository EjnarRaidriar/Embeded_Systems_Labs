#ifndef L298_HPP
#define L298_HPP

#include <Arduino.h>

void l298_setup(uint8_t ena_pin, uint8_t in1_pin, uint8_t in2_pin);
void l298_set(uint8_t ena_pin, uint8_t in1_pin, uint8_t in2_pin, int power_percent);

#endif
