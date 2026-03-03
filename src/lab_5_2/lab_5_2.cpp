#include "lab_5_2.hpp"

#if APP_NAME == LAB_5_2

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include "stdio_setups/serial_stdio/serail_stdio.hpp"

#include "lab_5_2_config.hpp"
#include "speed_task/speed_task.hpp"
#include "pid_task/pid_task.hpp"
#include "motor_task/motor_task.hpp"
#include "monitor_task/monitor_task.hpp"
#include "input_task/input_task.hpp"

QueueHandle_t rpm_queue;
QueueHandle_t pwm_queue;
QueueHandle_t monitor_queue;
QueueHandle_t setpoint_queue;

volatile long encoder_count = 0;

void encoder_isr() { encoder_count++; }

void lab_5_2_setup() {
	serial_setup();

	rpm_queue = xQueueCreate(QUEUE_SIZE, sizeof(float));
	pwm_queue = xQueueCreate(QUEUE_SIZE, sizeof(int));
	setpoint_queue = xQueueCreate(QUEUE_SIZE, sizeof(float));
	monitor_queue = xQueueCreate(QUEUE_SIZE, sizeof(monitor_data_t));

	if (rpm_queue == NULL || pwm_queue == NULL || setpoint_queue == NULL || monitor_queue == NULL) {
		printf("Error creating queue\n");
		while (1)
			;
	}

    attachInterrupt(digitalPinToInterrupt(PIN_MOTOR_ENCODER_C1), encoder_isr,
            RISING);

    xTaskCreate(pid_task, "PIDTask", 1024, NULL, 2, NULL);
    xTaskCreate(speed_task, "SpeedTask", 512, NULL, 2, NULL);
    xTaskCreate(input_task, "InputTask", 512, NULL, 1, NULL);
	xTaskCreate(motor_task, "MotorTask", 512, NULL, 1, NULL);
	xTaskCreate(monitor_task, "MonitorTask", 1024, NULL, 1, NULL);
}

void lab_5_2_loop() {}

#endif
