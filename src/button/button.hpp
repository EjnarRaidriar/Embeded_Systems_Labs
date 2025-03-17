#ifndef BUTTON_HPP
#define BUTTON_HPP

#define BUTTON_PIN 1

void button_setup();
int button_read();
bool button_is_pressed();

#endif