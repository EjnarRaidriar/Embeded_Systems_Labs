#include "lab_5_2.hpp"

#if APP_NAME == LAB_5_2

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include "stdio_setups/serial_stdio/serail_stdio.hpp"
#include "dht/dht.hpp"

#define DHT_PIN 2


void lab_5_2_setup() {
	serial_setup();
	relay_setup(RELAY_PIN);

	relayOnSemaphore = xSemaphoreCreateBinary();
	relayOffSemaphore = xSemaphoreCreateBinary();
	dataQueue = xQueueCreate(QUEUE_SIZE, sizeof(QUEUE_DATA_TYPE));

	if (relayOnSemaphore == NULL || relayOffSemaphore == NULL) {
		printf("Error creating semaphores\n");
		while (1)
			;
	}

	if (dataQueue == NULL) {
		printf("Error creating queue\n");
		while (1)
			;
	}

	xTaskCreate(input_task, "InputTask", 256, NULL, 1, NULL);
	xTaskCreate(relay_on_task, "RelayOnTask", 128, NULL, 1, NULL);
	xTaskCreate(relay_off_task, "RelayOffTask", 128, NULL, 1, NULL);
	xTaskCreate(monitor_task, "MonitorTask", 128, NULL, 1, NULL);
}

void lab_5_2_loop() {}

#include <semphr.h>

#include "relay/relay.hpp"

#define RELAY_PIN 2

#define MONITOR_TASK_REQ 200
#define INPUT_TASK_REQ 100

#define QUEUE_SIZE 1
#define QUEUE_DATA_TYPE bool

QueueHandle_t dataQueue;
SemaphoreHandle_t relayOnSemaphore;
SemaphoreHandle_t relayOffSemaphore;

void monitor_task(void *param);
void input_task(void *param);
void relay_on_task(void *param);
void relay_off_task(void *param);

void lab_4_1_setup() {
}

void lab_4_1_loop() {}

void input_task(void *param) {
	for (;;) {
		printf("Commands:\n");
		printf("\trelay on - turn on the relay\n");
		printf("\trelay off - turn off the rely\n");
		char command[25], option[25];
		scanf("%s %s", command, option);
		printf("\n");
		if (strcmp(command, "relay") == 0) {
			if (strcmp(option, "on") == 0) {
				xSemaphoreGive(relayOnSemaphore);
			} else if (strcmp(option, "off") == 0) {
				xSemaphoreGive(relayOffSemaphore);
			} else {
                printf("Unknown command\n");
            }
		} else {
			printf("Unknown command\n");
		}
		vTaskDelay(pdMS_TO_TICKS(INPUT_TASK_REQ));
	}
}

void relay_on_task(void *param) {
	for (;;) {
		if (xSemaphoreTake(relayOnSemaphore, portMAX_DELAY) == pdTRUE) {
			relay_NO_on(RELAY_PIN);
			bool data = true;
			xQueueOverwrite(dataQueue, &data);
		}
	}
}

void relay_off_task(void *param) {
	for (;;) {
		if (xSemaphoreTake(relayOffSemaphore, portMAX_DELAY) == pdTRUE) {
			relay_NO_off(RELAY_PIN);
			bool data = false;
			xQueueOverwrite(dataQueue, &data);
		}
	}
}

void monitor_task(void *param) {
	bool relay_is_on;
	for (;;) {
		if (xQueueReceive(dataQueue, &relay_is_on, portMAX_DELAY) == pdTRUE) {
			if (relay_is_on == true) {
				printf("Relay turned on\n");
			} else {
				printf("Relay turned off\n");
			}
		}
		vTaskDelay(pdMS_TO_TICKS(MONITOR_TASK_REQ));
	}
}

#endif

