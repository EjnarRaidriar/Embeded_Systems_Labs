#include "config.h"

#if APP_NAME == LAB_5_2

#include "monitor_task.hpp"
#include "lab_5_2/lab_5_2_config.hpp"

#include <Arduino.h>

void monitor_task(void *param) {
    monitor_data_t data;

    char sp_str[10];
    char rpm_str[10];

	for (;;) {
		if (xQueueReceive(monitor_queue, &data, portMAX_DELAY) == pdTRUE) {
            dtostrf(data.setpoint, 6, 2, sp_str);
            dtostrf(data.rpm, 6, 2, rpm_str);

            printf("%s,%s,%d,\n", sp_str, rpm_str, data.pwm);
		}
	}
}

#endif
