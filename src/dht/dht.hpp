#ifndef DHT_HPP
#define DHT_HPP

#include "DHT_U.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Arduino.h>

class DHT_11 {
private:
    DHT_Unified dht;

public:
    DHT_11(uint8_t pin): dht(DHT_Unified(pin, DHT11)) {}

    void setup();
    float read_humidity();
};

#endif
