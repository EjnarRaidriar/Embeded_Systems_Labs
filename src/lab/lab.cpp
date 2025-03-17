#include "lab/lab.hpp"

#include <Arduino.h>
#include <Keypad.h>

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define LCD_COLUMNS 16
#define LCD_ROWS 2

#define CLEAR_KEY '*'

LiquidCrystal_I2C lcd1(0x3F, LCD_COLUMNS, LCD_ROWS);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const byte ROWS1 = 3; //four rows
const byte COLS1 = 3; //three columns
char keys1[ROWS1][COLS1] = {
{'1','2','3'},
{'4','5','6'},
{'*','0','#'}
};
byte rowPins1[ROWS1] = {5, 4, 3}; //connect to the row pinouts of the kpd
byte colPins1[COLS1] = {8, 7, 6}; //connect to the column pinouts of the kpd

Keypad kpd1 = Keypad( makeKeymap(keys1), rowPins1, colPins1, ROWS1, COLS1 );


void lab_setup() {
  Serial.begin(9600);
  // lcd1.init();
  lcd1.backlight();
  lcd1.setBacklight(HIGH);
  lcd1.setCursor(5, 0);
}

void lab_loop() {
  char key = kpd1.getKey();
  if (key) {
    Serial.println(key);
    lcd1.setCursor(4, 1);
    lcd1.print(key);
  }
}