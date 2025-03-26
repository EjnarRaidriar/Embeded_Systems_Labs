#include "lab/lab.hpp"

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned int TBAdrr;

void lab_setup() {
  Serial.begin(9600);
  TCCR0A = B01000010;
  TCCR0B = B00000010;
  OCR0A = 128;
  TIMSK0 = B00000010;
  DDRB = DDRB | B10000000;
}

ISR(TIMER0_COMPA_vect) {
  TBAdrr++;
  Serial.println(TBAdrr, HEX);
}

void lab_loop() {
}