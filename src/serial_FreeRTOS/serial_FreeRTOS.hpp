#ifndef SERIAL_FREERTOS_HPP
#define SERIAL_FREERTOS_HPP

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

extern SemaphoreHandle_t serial_write_mutex;

void serial_FreeRTOS_setup();
int  serial_read_line(char *buf, int max_len);
void serial_print(const char *str);
void serial_println(const char *str);
void serial_print_int(int value);

#endif
