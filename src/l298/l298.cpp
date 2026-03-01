#include "l298.hpp"

void l298_setup(uint8_t ena_pin, uint8_t in1_pin, uint8_t in2_pin) {
    pinMode(ena_pin, OUTPUT);
    pinMode(in1_pin, OUTPUT);
    pinMode(in2_pin, OUTPUT);

    digitalWrite(in1_pin, LOW);
    digitalWrite(in2_pin, LOW);
    analogWrite(ena_pin, 0);
}

void l298_set(uint8_t ena_pin, uint8_t in1_pin, uint8_t in2_pin, int power_percent) {
    int pwm = map(abs(power_percent), 0, 100, 0, 255);

    if (power_percent > 0) {
        digitalWrite(in1_pin, HIGH);
        digitalWrite(in2_pin, LOW);
    } else if (power_percent < 0) {
        digitalWrite(in1_pin, LOW);
        digitalWrite(in2_pin, HIGH);
    } else {
        digitalWrite(in1_pin, LOW);
        digitalWrite(in2_pin, LOW);
    }
    analogWrite(ena_pin, pwm);
}
