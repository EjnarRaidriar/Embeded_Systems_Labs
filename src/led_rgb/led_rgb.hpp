#ifndef LED_RGB_HPP
#define LED_RGB_HPP

#include <Arduino.h>

void led_RGB_setup(uint8_t r_pin, uint8_t g_pin, uint8_t b_pin);
void led_RGB_turn_on(const int &led);
void led_RGB_turn_off(const int &led);
int led_RGB_read(const int &led);
void led_RGB_write(const int &led, uint8_t level);
bool led_RGB_is_on(const int &led);
void led_RGB_toggle(const int &led);

#endif
