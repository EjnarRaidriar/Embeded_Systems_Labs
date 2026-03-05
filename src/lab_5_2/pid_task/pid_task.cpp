#include "config.h"

#if APP_NAME == LAB_5_2

#include "pid_task.hpp"
#include "lab_5_2/lab_5_2_config.hpp"


void pid_task(void *parma) {
	float setpoint = DEFAULT_SETPOINT;
	float integral = 0.0f;
	float last_error = 0.0f;
	float measured;

	for (;;) {
        float new_setpoint;
        if (xQueueReceive(setpoint_queue, &new_setpoint, 0) == pdTRUE) {
            setpoint = new_setpoint;
        }

		if (xQueueReceive(rpm_queue, &measured, portMAX_DELAY) == pdTRUE) {
			float error = setpoint - measured;

			integral += error * SAMPLE_TIME_S;
            if (integral > PWM_MAX) integral = PWM_MAX;
            if (integral < PWM_MIN) integral = PWM_MIN;
			float derivative = (error - last_error) / SAMPLE_TIME_S;

			float output =
                PID_KP * error +
                PID_KI * integral +
                PID_KD * derivative;

			last_error = error;

			if (output > PWM_MAX)
				output = PWM_MAX;
			if (output < PWM_MIN)
				output = PWM_MIN;

			int pwm = (int)output;
			xQueueSend(pwm_queue, &pwm, 0);

            monitor_data_t data;
            data.setpoint = setpoint;
            data.rpm = measured;
            data.pwm = pwm;

            xQueueSend(monitor_queue, &data, 0);
		}
	}
}

#endif
