#import "dht.hpp"

void DHT_11::setup() {
    this->dht.begin();
}

float DHT_11::read_humidity() {
    sensors_event_t event;
    this->dht.humidity().getEvent(&event);
    return event.relative_humidity;
}
