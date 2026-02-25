#ifndef LED_V2_HPP
#define LED_V2_HPP

#include <Arduino.h>

void led_v2_setup(const int &led);
void led_v2_turn_on(const int &led);
void led_v2_turn_off(const int &led);
int led_v2_read(const int &led);
void led_v2_write(const int &led, uint8_t level);
bool led_v2_is_on(const int &led);
void led_v2_toggle(const int &led);

#endif
