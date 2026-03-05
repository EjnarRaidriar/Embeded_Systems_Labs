#include "config.h"

#if APP_NAME == LAB_5_2

#include "motor_task.hpp"
#include "lab_5_2/lab_5_2_config.hpp"

#include "l298/l298.hpp"

void motor_task(void *param) {
	l298_setup(L298_ENA, L298_IN1, L298_IN2);
	int pwm;
	for (;;) {
		if (xQueueReceive(pwm_queue, &pwm, portMAX_DELAY) == pdTRUE) {
			l298_set(L298_ENA, L298_IN1, L298_IN2, pwm);
		}
	}
}

#endif
