#include "config.h"

#if APP_NAME == LAB_5_2

#include "input_task.hpp"
#include "lab_5_2/lab_5_2_config.hpp"

#include "potentiometer_5k/potentiometer_5k.hpp"

#include <Arduino.h>

void input_task(void *param) {
	float setpoint = DEFAULT_SETPOINT;
	TickType_t last_step_tick = xTaskGetTickCount();

	for (;;) {
		int raw = read_potentiometer(PIN_POTENTIOMETER);

		if (raw > POT_HIGH_THRESHOLD) {
			if ((xTaskGetTickCount() - last_step_tick) >=
				pdMS_TO_TICKS(SETPOINT_STEP_INTERVAL_MS)) {
				setpoint += SETPOINT_STEP;
				if (setpoint > SETPOINT_MAX)
					setpoint = SETPOINT_MAX;
				xQueueOverwrite(setpoint_queue, &setpoint);
				last_step_tick = xTaskGetTickCount();
			}
		} else if (raw < POT_LOW_THRESHOLD) {
			if ((xTaskGetTickCount() - last_step_tick) >=
				pdMS_TO_TICKS(SETPOINT_STEP_INTERVAL_MS)) {
				setpoint -= SETPOINT_STEP;
				if (setpoint < SETPOINT_MIN)
					setpoint = SETPOINT_MIN;
				xQueueOverwrite(setpoint_queue, &setpoint);
				last_step_tick = xTaskGetTickCount();
			}
		} else {
			last_step_tick = xTaskGetTickCount();
		}

		vTaskDelay(pdMS_TO_TICKS(INPUT_TASK_REQ));
	}
}

#endif
