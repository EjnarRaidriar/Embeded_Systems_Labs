#ifndef LCD_STDIO_HPP
#define LCD_STDIO_HPP

#include <stdio.h>

void lcdSetup();
int lcdPutChar(char c, FILE *f);
void setCursor(int col, int row);
void resetScreen();

#endif