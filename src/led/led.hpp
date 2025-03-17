#ifndef LED_HPP
#define LED_HPP

#define LED_PIN 13

void led_setup();
void led_turn_on();
void led_turn_off();
int led_read();
bool led_is_on();
void led_toggle();

#endif