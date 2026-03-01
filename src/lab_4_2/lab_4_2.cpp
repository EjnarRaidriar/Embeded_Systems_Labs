#include "lab_4_2.hpp"
#include "l298/l298.hpp"

#if APP_NAME == LAB_4_2

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include "stdio_setups/serial_stdio/serail_stdio.hpp"

#define L298_ENA 5
#define L298_IN1 8
#define L298_IN2 9

#define MONITOR_TASK_REQ 100
#define INPUT_TASK_REQ 200

#define QUEUE_SIZE 3
#define QUEUE_DATA_TYPE int

QueueHandle_t commandQueue;
QueueHandle_t stateQueue;

SemaphoreHandle_t serialMutex;

void monitor_task(void *param);
void input_task(void *param);
void motor_task(void *param);

void safe_printf(const char *format, ...) {
	xSemaphoreTake(serialMutex, portMAX_DELAY);

	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	xSemaphoreGive(serialMutex);
}

void lab_4_2_setup() {
	serial_setup();

	serialMutex = xSemaphoreCreateMutex();

	commandQueue = xQueueCreate(QUEUE_SIZE, sizeof(QUEUE_DATA_TYPE));
	stateQueue = xQueueCreate(QUEUE_SIZE, sizeof(QUEUE_DATA_TYPE));

	if (commandQueue == NULL || stateQueue == NULL) {
		safe_printf("Error creating queue\n");
		while (1)
			;
	}

	xTaskCreate(input_task, "InputTask", 1024, NULL, 1, NULL);
	xTaskCreate(motor_task, "MotorTask", 1024, NULL, 1, NULL);
	xTaskCreate(monitor_task, "MonitorTask", 1024, NULL, 1, NULL);
}

void lab_4_2_loop() {}

void print_help() {
	safe_printf("\nAvailable commands:\n");
	safe_printf("  motor set [-100..100]\n");
	safe_printf("  motor stop\n");
	safe_printf("  motor max\n");
	safe_printf("  motor inc\n");
	safe_printf("  motor dec\n");
	safe_printf("  help\n\n");
}

void input_task(void *param)
{
    int power = 0;

    print_help();

    for (;;)
    {
        safe_printf("> ");

        char command[20];

        if (scanf("%19s", command) != 1)
            continue;

        if (strcmp(command, "help") == 0)
        {
            print_help();
            continue;
        }

        if (strcmp(command, "motor") != 0)
        {
            safe_printf("Unknown command\n");
            continue;
        }

        char option[20];

        if (scanf("%19s", option) != 1)
        {
            safe_printf("Missing option\n");
            continue;
        }

        if (strcmp(option, "set") == 0)
        {
            safe_printf("Enter value [-100..100]: ");

            int value;
            if (scanf("%d", &value) != 1)
            {
                safe_printf("Invalid number\n");
                continue;
            }

            if (value < -100) value = -100;
            if (value > 100)  value = 100;

            power = value;
        }
        else if (strcmp(option, "stop") == 0)
        {
            power = 0;
        }
        else if (strcmp(option, "max") == 0)
        {
            power = (power >= 0) ? 100 : -100;
        }
        else if (strcmp(option, "inc") == 0)
        {
            power += 10;
            if (power > 100) power = 100;
        }
        else if (strcmp(option, "dec") == 0)
        {
            power -= 10;
            if (power < -100) power = -100;
        }
        else
        {
            safe_printf("Unknown option\n");
            continue;
        }

        xQueueSend(commandQueue, &power, portMAX_DELAY);
    }
}

void motor_task(void *param) {
	l298_setup(L298_ENA, L298_IN1, L298_IN2);
	int power = 0;
	for (;;) {
		if (xQueueReceive(commandQueue, &power, portMAX_DELAY) == pdTRUE) {
			l298_set(L298_ENA, L298_IN1, L298_IN2, power);
			xQueueSend(stateQueue, &power, portMAX_DELAY);
		}
	}
}

void monitor_task(void *param) {
	QUEUE_DATA_TYPE received_power;
	for (;;) {
		if (xQueueReceive(stateQueue, &received_power, portMAX_DELAY) == pdTRUE) {
			safe_printf("Power: %d%%\n", received_power);
			safe_printf("> ");
		}
		vTaskDelay(pdMS_TO_TICKS(MONITOR_TASK_REQ));
	}
}

#endif
