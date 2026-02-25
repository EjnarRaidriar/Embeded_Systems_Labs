#include "photoresistor.hpp"

int photoresistor_read(uint8_t pin) {
    return analogRead(pin);
}

