#include "lcd_stdio.hpp"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define LCD_COLUMNS 16
#define LCD_ROWS 2

#define CLEAR_KEY '*'

FILE *lcdStream;

LiquidCrystal_I2C lcd(0x3f, LCD_COLUMNS, LCD_ROWS);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void lcdSetup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  lcdStream = fdevopen(lcdPutChar, NULL);
  stdout = lcdStream;
}

int lcdPutChar(char c, FILE *f) {
  if (c == CLEAR_KEY) {
    lcd.clear();
    lcd.setCursor(0, 0);
  }
  lcd.print(c);
  return 0;
}

void setCursor(int col, int row) {
  lcd.setCursor(col, row);
}

void resetScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
}