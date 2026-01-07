#ifndef FILTER_HPP
#define FILTER_HPP

#include <Arduino.h>

long map(int x, long in_min, long in_max, long out_min, long out_max);

#define BUFFER_SIZE 5

class Filter {
private:
    long buffer[BUFFER_SIZE];
    uint8_t index;
    bool filled;

public:
    Filter(): index(0), filled(false) {}

    void add(long value);
    bool ready() const;

    long median();
    long wighted_average(const int weights[BUFFER_SIZE]);
};

#endif
