#include "serial_FreeRTOS.hpp"

SemaphoreHandle_t serial_write_mutex;

void serial_FreeRTOS_setup() {
    Serial.begin(9600);
    serial_write_mutex = xSemaphoreCreateMutex();
}

void serial_print(const char *str) {
    xSemaphoreTake(serial_write_mutex, portMAX_DELAY);
    Serial.print(str);
    xSemaphoreGive(serial_write_mutex);
}

void serial_println(const char *str) {
    xSemaphoreTake(serial_write_mutex, portMAX_DELAY);
    Serial.println(str);
    xSemaphoreGive(serial_write_mutex);
}

void serial_print_int(int value) {
    xSemaphoreTake(serial_write_mutex, portMAX_DELAY);
    Serial.print(value);
    xSemaphoreGive(serial_write_mutex);
}

// Reads one line, blocks until '\n', echoes each char back, returns length
int serial_read_line(char *buf, int max_len) {
    int i = 0;
    while (i < max_len - 1) {
        while (!Serial.available()) {
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        char c = Serial.read();
        if (c == '\r') continue;   // ignore carriage return
        if (c == '\n') break;      // end of line
        // handle backspace
        if (c == 0x08 || c == 0x7F) {
            if (i > 0) {
                i--;
                xSemaphoreTake(serial_write_mutex, portMAX_DELAY);
                Serial.print("\b \b");
                xSemaphoreGive(serial_write_mutex);
            }
            continue;
        }
        buf[i++] = c;
        xSemaphoreTake(serial_write_mutex, portMAX_DELAY);
        Serial.write(c);           // echo
        xSemaphoreGive(serial_write_mutex);
    }
    buf[i] = '\0';
    xSemaphoreTake(serial_write_mutex, portMAX_DELAY);
    Serial.println();              // newline after enter
    xSemaphoreGive(serial_write_mutex);
    return i;
}
