#include "lab_6_1.hpp"

#if APP_NAME == LAB_6_1

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <queue.h>

#include "button_v2/button_v2.hpp"
#include "led_v2/led_v2.hpp"
#include "stdio_setups/serial_stdio/serail_stdio.hpp"

#define LED_PIN 2
#define BUTTON_PIN 3

#define STATE_OFF 0
#define STATE_WAITING 1
#define STATE_ON 2
#define STATE_BLINK_ON 3
#define STATE_BLINK_OFF 4

#define WAIT_TIME 500
#define BLINK_PERIOD 100

typedef struct {
	unsigned long Out;
	unsigned long Time;
	unsigned long Next[2];
} State_t;

const State_t FSM[] = {
	{0, 0, {STATE_OFF, STATE_WAITING}},								// OFF
	{1, pdMS_TO_TICKS(WAIT_TIME), {STATE_ON, STATE_BLINK_ON}},		// WAITING
	{1, 0, {STATE_ON, STATE_OFF}},									// ON
	{1, pdMS_TO_TICKS(BLINK_PERIOD), {STATE_BLINK_OFF, STATE_OFF}}, // BLINK_ON
	{0, pdMS_TO_TICKS(BLINK_PERIOD), {STATE_BLINK_ON, STATE_OFF}},	// BLINK_OFF
};

#define BUTTON_DEBOUNCE_TIME 100
#define BUTTON_TASK_REQ 50
#define FSM_TASK_REQ 10
#define MONITOR_TASK_REQ 400

#define QUEUE_SIZE 5
#define QUEUE_DATA_TYPE uint8_t
#define MONITOR_QUEUE_SIZE 1
#define MONITOR_QUEUE_TYPE uint8_t

QueueHandle_t buttonQueue;
QueueHandle_t monitorQueue;

void button_task(void *param);
void fsm_task(void *param);
void monitor_task(void *param);

void lab_6_1_setup() {
	serial_setup();

	buttonQueue = xQueueCreate(QUEUE_SIZE, sizeof(QUEUE_DATA_TYPE));
	monitorQueue = xQueueCreate(MONITOR_QUEUE_SIZE, sizeof(MONITOR_QUEUE_TYPE));

	if (buttonQueue == NULL) {
		printf("Error creating queue\n");
		while (1)
			;
	}

	xTaskCreate(button_task, "ButtonTask", 128, NULL, 1, NULL);
	xTaskCreate(fsm_task, "FSMTask", 256, NULL, 1, NULL);
	xTaskCreate(monitor_task, "MonitorTask", 128, NULL, 1, NULL);
}

void lab_6_1_loop() {}

void button_task(void *param) {
	button_v2_setup(BUTTON_PIN);
	bool last_state = HIGH;

	for (;;) {
		bool current_state = button_v2_read(BUTTON_PIN);

		if (current_state == LOW && last_state == HIGH) {
			uint8_t event = 1;
			xQueueSend(buttonQueue, &event, 0);
			vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBOUNCE_TIME));
		}
		last_state = current_state;
		vTaskDelay(pdMS_TO_TICKS(BUTTON_TASK_REQ));
	}
}

void fsm_task(void *param) {
	led_v2_setup(LED_PIN);
	uint8_t current_state = STATE_OFF;
	TickType_t state_start_tick = xTaskGetTickCount();
	uint8_t event;

	for (;;) {
		led_v2_write(LED_PIN, FSM[current_state].Out);

		if (xQueueReceive(buttonQueue, &event, 0) == pdTRUE) {
			current_state = FSM[current_state].Next[1];
			state_start_tick = xTaskGetTickCount();
			xQueueSend(monitorQueue, &current_state, 0);
		} else if (FSM[current_state].Time > 0) {
			if ((xTaskGetTickCount() - state_start_tick) >=
				FSM[current_state].Time) {
				current_state = FSM[current_state].Next[0];
				state_start_tick = xTaskGetTickCount();
				xQueueSend(monitorQueue, &current_state, 0);
			}
		}
		vTaskDelay(pdMS_TO_TICKS(FSM_TASK_REQ));
	}
}

void monitor_task(void *param) {
	MONITOR_QUEUE_TYPE state;
	for (;;) {
		if (xQueueReceive(monitorQueue, &state, portMAX_DELAY) == pdTRUE) {
			switch (state) {
			case STATE_OFF:
				printf("OFF\n");
				break;
			case STATE_ON:
				printf("ON\n");
				break;
			case STATE_WAITING:
				printf("WAITING\n");
				break;
			case STATE_BLINK_ON:
				printf("BLINK_ON\n");
				break;
			case STATE_BLINK_OFF:
				printf("BLINK_OFF\n");
				break;
			default:
				printf("Error: Unknown state\n");
				break;
			}
		}
		// vTaskDelay(pdMS_TO_TICKS(MONITOR_TASK_REQ));
	}
}

#endif
