#include "relay.hpp"

#include <Arduino.h>

void relay_setup(const int &relay_pin) {
    pinMode(relay_pin, OUTPUT);
}

void relay_NO_on(const int &relay_pin) {
    digitalWrite(relay_pin, HIGH);
}
void relay_NO_off(const int &relay_pin) {
    digitalWrite(relay_pin, LOW);
}
