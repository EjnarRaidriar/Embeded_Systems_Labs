#include "speed_task.hpp"
#include "lab_5_2/lab_5_2_config.hpp"

#include <Arduino.h>

void speed_task(void *param) {

	long last_count = 0;

	for (;;) {
		vTaskDelay(pdMS_TO_TICKS(SPEED_TASK_REQ));

        noInterrupts();
		long current_count = encoder_count;
        interrupts();
		long delta = current_count - last_count;
		last_count = current_count;

		float rpm =
			(delta / (float)ENCODER_PULSES_PER_REV) * (60.0 / SAMPLE_TIME_S);

		xQueueSend(rpm_queue, &rpm, 0);
	}
}
