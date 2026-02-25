#include "lab_6_2.hpp"
#include "photoresistor/photoresistor.hpp"

#if APP_NAME == LAB_6_2

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <queue.h>

#include "button_v2/button_v2.hpp"
#include "led_v2/led_v2.hpp"
#include "stdio_setups/serial_stdio/serail_stdio.hpp"

#define LED_N_R_PIN 8
#define LED_N_G_PIN 9

#define LED_E_R_PIN 4
#define LED_E_G_PIN 3

#define BUTTON_PIN 13
#define PHOTO_RES_PIN A0

#define NIGHT_THRESHOLD 300

#define STATE_GO_NORTH 0
#define STATE_GO_NORTH_UNTOUCHED 1
#define STATE_WAIT_NORTH 2 // before north can go
#define STATE_GO_EAST 3
#define STATE_WAIT_EAST 4 // before east can go
#define STATE_NIGHT_BLINK_ON 5
#define STATE_NIGHT_BLINK_OFF 6

#define NORTH_GREEN_TIME 5000
#define EAST_GREEN_TIME 4000
#define YELLOW_ON_OFF_TIME 1500

typedef struct {
	unsigned long out;
	unsigned long time;
	uint8_t next[3];
} State_t;

// Sequence of the lights for state output:
// North: R, Y, G   East: R, Y, G
//
// Table columns:
// output, timeout, new state
// new states:
// 0 - timeout && is day
// 1 - timeout && is night
// 2 - timeout && button was pressed
const State_t FSM[] = {
	{0b001100,
	 0,
	 {STATE_GO_NORTH, STATE_NIGHT_BLINK_ON, STATE_WAIT_EAST}}, // go north
	{0b001100,
	 pdMS_TO_TICKS(NORTH_GREEN_TIME),
	 {STATE_GO_NORTH, STATE_NIGHT_BLINK_ON,
	  STATE_WAIT_EAST}}, // go north untouched
	{0b010010,
	 pdMS_TO_TICKS(YELLOW_ON_OFF_TIME),
	 {STATE_GO_NORTH_UNTOUCHED, STATE_NIGHT_BLINK_OFF,
	  STATE_GO_NORTH_UNTOUCHED}}, // wait north
	{0b100001,
	 pdMS_TO_TICKS(EAST_GREEN_TIME),
	 {STATE_WAIT_NORTH, STATE_NIGHT_BLINK_ON, STATE_WAIT_NORTH}}, // go east
	{0b010010,
	 pdMS_TO_TICKS(YELLOW_ON_OFF_TIME),
	 {STATE_GO_EAST, STATE_NIGHT_BLINK_OFF, STATE_GO_EAST}}, // wait east
	{0b010010,
	 pdMS_TO_TICKS(YELLOW_ON_OFF_TIME),
	 {STATE_GO_NORTH, STATE_NIGHT_BLINK_OFF,
	  STATE_NIGHT_BLINK_OFF}}, // night blink on
	{0b000000,
	 pdMS_TO_TICKS(YELLOW_ON_OFF_TIME),
	 {STATE_GO_NORTH, STATE_NIGHT_BLINK_ON,
	  STATE_NIGHT_BLINK_ON}}, // night blink off
};

#define BUTTON_DEBOUNCE_TIME 100
#define BUTTON_TASK_REQ 50
#define FSM_TASK_REQ 100
#define MONITOR_TASK_REQ 400

#define QUEUE_SIZE 1
#define QUEUE_DATA_TYPE uint8_t

QueueHandle_t sensorQueue;
QueueHandle_t monitorQueue;
QueueHandle_t eventLogQueue;

void sensor_task(void *param);
void fsm_task(void *param);
void monitor_task(void *param);

void lab_6_2_setup() {
	serial_setup();

	sensorQueue = xQueueCreate(QUEUE_SIZE, sizeof(QUEUE_DATA_TYPE));
	monitorQueue = xQueueCreate(QUEUE_SIZE, sizeof(QUEUE_DATA_TYPE));
	eventLogQueue = xQueueCreate(5, sizeof(QUEUE_DATA_TYPE));

	if (sensorQueue == NULL || monitorQueue == NULL || eventLogQueue == NULL) {
		printf("Error creating queue\n");
		while (1)
			;
	}

	xTaskCreate(sensor_task, "SensorTask", 128, NULL, 1, NULL);
	xTaskCreate(fsm_task, "FSMTask", 256, NULL, 1, NULL);
	xTaskCreate(monitor_task, "MonitorTask", 256, NULL, 1, NULL);
}

void lab_6_2_loop() {}

void set_leds(uint8_t pattern) {
	uint8_t nr = (pattern & 0b100000) || (pattern & 0b010000);
	uint8_t ng = (pattern & 0b001000) || (pattern & 0b010000);
	led_v2_write(LED_N_R_PIN, nr);
	led_v2_write(LED_N_G_PIN, ng);

	uint8_t er = (pattern & 0b000100) || (pattern & 0b000010);
	uint8_t eg = (pattern & 0b000001) || (pattern & 0b000010);
	led_v2_write(LED_E_R_PIN, er);
	led_v2_write(LED_E_G_PIN, eg);
}

void sensor_task(void *param) {
	button_v2_setup(BUTTON_PIN);
	bool last_state = HIGH;

	for (;;) {
		bool current_state = button_v2_read(BUTTON_PIN);
		int light_value = photoresistor_read(PHOTO_RES_PIN);
		uint8_t event_code = 0;

		if (current_state == LOW && last_state == HIGH) {
			event_code = 2;
		}

		if (light_value < NIGHT_THRESHOLD) {
			event_code = 1;
		}

		if (event_code != 0) {
			xQueueOverwrite(sensorQueue, &event_code);
			xQueueSend(eventLogQueue, &event_code, 0);
		}
		last_state = current_state;
		vTaskDelay(pdMS_TO_TICKS(BUTTON_TASK_REQ));
	}
}

void fsm_task(void *param) {
	led_v2_setup(LED_N_R_PIN);
	led_v2_setup(LED_N_G_PIN);
	led_v2_setup(LED_E_R_PIN);
	led_v2_setup(LED_E_G_PIN);
	uint8_t state = STATE_GO_NORTH;
	uint8_t last_state = state;
	uint8_t event = 0;

	for (;;) {
		set_leds(FSM[state].out);
		if (FSM[state].time > 0) {
			vTaskDelay(FSM[state].time);
		}

		if (xQueueReceive(sensorQueue, &event, 0) == pdTRUE) {
			state = FSM[state].next[event];
		} else {
			state = FSM[state].next[0];
		}
		if (state != last_state) {
			xQueueSend(monitorQueue, &state, 0);
		}
		last_state = state;
		vTaskDelay(pdMS_TO_TICKS(FSM_TASK_REQ));
	}
}

void monitor_task(void *param) {
	uint8_t state;
	uint8_t event;

	for (;;) {
		// 1. Check for State Changes (from FSM)
		if (xQueueReceive(monitorQueue, &state, 0) == pdTRUE) {
			printf("[STATE CHANGE] Current:");
			// You can add logic here to print specific names based on the index
			switch (state) {
			case STATE_GO_NORTH:
				printf("GO_NORTH (N: Green, E: Red)\n");
				break;
			case STATE_GO_NORTH_UNTOUCHED:
				printf("GO_NORTH_UNTOUCHED (N: Green, E: Red)\n");
				break;
			case STATE_WAIT_NORTH:
				printf("WAIT_NORTH (Yellow Transition)\n");
				break;
			case STATE_GO_EAST:
				printf("GO_EAST (E: Green, N: Red)\n");
				break;
			case STATE_WAIT_EAST:
				printf("WAIT_EAST (Yellow Transition)\n");
				break;
			case STATE_NIGHT_BLINK_ON:
				printf("NIGHT_MODE (Yellow ON)\n");
				break;
			case STATE_NIGHT_BLINK_OFF:
				printf("NIGHT_MODE (Yellow OFF)\n");
				break;
			default:
				printf("Error: Unknown state %d\n", state);
				break;
			}
		}

		// 2. Check for Hardware Events (from Sensors)
		if (xQueueReceive(eventLogQueue, &event, 0) == pdTRUE) {
			printf(">>> [EVENT DETECTED] Code: %d ", event);
			if (event == 1)
				printf("(Night Detected)\n");
			if (event == 2)
				printf("(Button Pressed)\n");
		}

		vTaskDelay(pdMS_TO_TICKS(100)); // Polling the log queues
	}
}

#endif
