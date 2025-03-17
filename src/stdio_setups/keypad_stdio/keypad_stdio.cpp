#include "keypad_stdio.hpp"
#include "stdio_setups/lcd_stdio/lcd_stdio.hpp"

#include <Keypad.h>

#define KEYPAD_DELAY 100

FILE *keypadStream;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
// {'7','8','9'},
{'7','8','#'},
{'*','0','#'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {6, 7, 8}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void keypadSetup() {
  keypadStream = fdevopen(NULL, keypadGetChar);
  stdin = keypadStream;
}

int keypadGetChar(FILE *f) {
  char key;
  do {
    key = kpd.getKey();
  } while (key == NO_KEY);
  if (key == '#') {
    key = '\n';
  }
  lcdPutChar(key, NULL);
  delay(KEYPAD_DELAY);
  return key;
}