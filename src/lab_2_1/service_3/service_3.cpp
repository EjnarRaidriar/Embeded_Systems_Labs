#include "service_3.hpp"
#include "../global_var.hpp"

byte inc_button_state = LOW;
byte inc_button_last_state = LOW;
byte dec_button_state = LOW;
byte dec_button_last_state = LOW;

void service_3_setup() {
  pinMode(INC_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DEC_BUTTON_PIN, INPUT_PULLUP);
  inc_button_last_state = digitalRead(INC_BUTTON_PIN);
  dec_button_last_state = digitalRead(DEC_BUTTON_PIN);
}

void service_3_loop() {
  inc_button_state = digitalRead(INC_BUTTON_PIN);
  if (inc_button_state != inc_button_last_state) {
    inc_button_last_state = inc_button_state;
    if (inc_button_state == LOW) {
      if (recurrence < MAX) {
        recurrence++;
      }
    }
  }

  dec_button_state = digitalRead(DEC_BUTTON_PIN);
  if (dec_button_state != dec_button_last_state) {
    dec_button_last_state = dec_button_state;
    if (dec_button_state == LOW) {
      if (recurrence > 1) {
        recurrence--;
      }
    }
  }
}