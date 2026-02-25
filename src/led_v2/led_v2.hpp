#ifndef LED_V2_HPP
#define LED_V2_HPP

#include <Arduino.h>

void led_v2_setup(const int &led);
void led_v2_turn_on(const int &led);
void led_v2_turn_off(const int &led);
int led_v2_read(const int &led);
void led_v2_write(const int &led, uint8_t level);
void led_v2_write_rgb(uint8_t r_pin, uint8_t g_pin, uint8_t b_pin,
					  uint8_t r_level, uint8_t g_level, uint8_t b_level);
bool led_v2_is_on(const int &led);
void led_v2_toggle(const int &led);

#endif
