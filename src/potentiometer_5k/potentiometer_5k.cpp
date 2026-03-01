#include "potentiometer_5k.hpp"

int read_potentiometer(uint8_t pin) {
    return analogRead(pin);
}
