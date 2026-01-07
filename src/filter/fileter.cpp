#include "filter.hpp"

long map(int x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool Filter::ready() const {
    return filled;
}

void Filter::add(long value) {
    buffer[index] = value;
    index = (index + 1) % BUFFER_SIZE;
    if (index == 0) filled = true;
}

long Filter::median() {
    uint8_t count = filled ? BUFFER_SIZE : index;
    if (count == 0) return 0;

    long temp[BUFFER_SIZE];
    for (uint8_t i = 0; i < count; i++)
        temp[i] = buffer[i];

    for (uint8_t i = 1; i < count; i++) {
        int key = temp[i];
        int j = i - 1;
        while (j >= 0 && temp[j] > key) {
            temp[j + 1] = temp[j];
            j--;
        }
        temp[j + 1] = key;
    }
    return temp[count / 2];
}

long Filter::wighted_average(const int weights[BUFFER_SIZE]) {
    int count = filled ? BUFFER_SIZE : index;
    if (count == 0) return 0;

    long long weighted_sum = 0;
    long long weight_total = 0;

    for (uint8_t i = 0; i < count; i++) {
        weighted_sum += (long long)buffer[i] * weights[i];
        weight_total += weights[i];
    }
    if (weight_total == 0) return 0;
    return (long)(weighted_sum / weight_total);
}
