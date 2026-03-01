#include <Arduino.h>

#include "serail_stdio.hpp"

FILE *serial_stdio_stream;

void serial_setup() {
  Serial.begin(9600);
  serial_stdio_stream = fdevopen(serial_put_char, serial_get_char);
  stdin = stdout = stderr = serial_stdio_stream;
}

int serial_get_char(FILE *stream) {
  char c;
  while (!Serial.available());
  c = Serial.read();
  Serial.write(c);
  return c;
}

int serial_put_char(char c, FILE *stream) {
  Serial.write(c);
  return 0;
}
