#include "led_v2.hpp"

#include <Arduino.h>

void led_v2_setup(const int &led) { pinMode(led, OUTPUT); }

void led_v2_turn_on(const int &led) { digitalWrite(led, HIGH); }

void led_v2_turn_off(const int &led) { digitalWrite(led, LOW); }

int led_v2_read(const int &led) { return digitalRead(led); }

void led_v2_write(const int &led, uint8_t level) { digitalWrite(led, level); }

void led_v2_write_rgb(uint8_t r_pin, uint8_t g_pin, uint8_t b_pin,
					  uint8_t r_level, uint8_t g_level, uint8_t b_level) {
	digitalWrite(r_pin, r_level);
	digitalWrite(g_pin, g_level);
	digitalWrite(b_pin, b_level);
}

bool led_v2_is_on(const int &led) { return led_v2_read(led) == HIGH; }

void led_v2_toggle(const int &led) {
	if (led_v2_is_on(led)) {
		led_v2_turn_off(led);
	} else {
		led_v2_turn_on(led);
	}
}
