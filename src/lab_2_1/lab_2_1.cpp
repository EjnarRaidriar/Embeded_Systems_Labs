#include "lab_2_1.hpp"

#include "global_var.hpp"

#include "service_1/service_1.hpp"
#include "service_2/service_2.hpp"
#include "service_3/service_3.hpp"
#include "stdio_setups/serial_stdio/serail_stdio.hpp"

byte led_1_state = LOW;
byte led_2_state = LOW;
int recurrence = 1;

#define SYS_TIC 300

int srv_1_recurrence = 500 / SYS_TIC;
int srv_1_counter = srv_1_recurrence;

int srv_2_recurrence = 1000 / SYS_TIC;
int srv_2_counter = srv_2_recurrence;

int srv_3_recurrence = 500 / SYS_TIC;
int srv_3_counter = srv_3_recurrence;

void setup_timer1() {
  noInterrupts();
  // clear time registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  // X (ms) = ((OCR1A + 1) * Prescaler) / (Clock Speed / 1000)
  OCR1A = 2500; // 10 ms at 16MHz
  TCCR1B |= (1 << WGM12); // CTC mode (Clear Timer on Compare Match)
  TCCR1B |= (1 << CS11) | (1 << CS10); // Prescaler 64
  TIMSK1 |= (1 << OCIE1A); // enable time compare interrupt
  interrupts();
}


void lab_2_1_setup() {
  Serial.begin(9600);
  setup_timer1();
  serial_setup();
  service_1_setup();
  service_2_setup();
  service_3_setup();
}

void time_scheduler() {
  if (--srv_1_counter <= 0) {
    service_1_loop();
    srv_1_counter = srv_1_recurrence;
  }
  if (--srv_2_counter <= 0) {
    service_2_loop();
    srv_2_counter = srv_2_recurrence;
  }
  if (--srv_3_counter <= 0) {
    service_3_loop();
    srv_3_counter = srv_3_recurrence;
  }
}

ISR(TIMER1_COMPA_vect) {
  time_scheduler();
}

void lab_2_1_loop() {
  printf("Led 1: %d\n", led_1_state);
  printf("Led 2: %d\n", led_2_state);
  printf("mult:  %d\n\n", recurrence);
  delay(SYS_TIC);
}