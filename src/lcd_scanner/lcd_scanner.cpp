#include "lcd_scanner.hpp"

#include "stdio_setups/serial_stdio/serail_stdio.hpp"

#include <Arduino.h>
#include <Wire.h>

void adress_println(byte &adress) {
  if (adress < 16) {
    printf("0");
  }
  printf("%x\n", adress);
}

void lcd_scanner_setup() {
  serial_setup();
  Wire.begin();
  Serial.begin(9600);
  while(!Serial);
}

void lcd_scanner_loop() {
  byte error, adress;
  int nDevices;
  printf("Scanning...\n");

  nDevices = 0;
  for (adress = 1; adress < 127; adress++) {
    Wire.beginTransmission(adress);
    error = Wire.endTransmission();

    if (error == 0) {
      printf("I2C device found at adress 0x");
      adress_println(adress);
      nDevices++;
    }
    else if (error == 4) {
      printf("Unknown error at adress 0x");
      adress_println(adress);
    }
  }
  if (nDevices == 0) {
    printf("No I2C devices found\n");
  }
  else {
    printf("done\n");
  }
  delay(5000);
}