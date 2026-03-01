#include "lab_5_1.hpp"

#if APP_NAME == LAB_5_1

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include "potentiometer_5k/potentiometer_5k.hpp"
#include "dht/dht.hpp"
#include "relay/relay.hpp"
#include "stdio_setups/serial_stdio/serail_stdio.hpp"

#define DHT_PIN 4
#define RELAY_PIN 3
#define POT_PIN A0

#define FIXED_SETPOINT 50.0f
#define MAX_HYSTEREZIS 10.0f

#define SENSOR_TASK_REQ 1000
#define CONTROL_TASK_REQ 500
#define MONITOR_TASK_REQ 500

struct SystemState {
	float humidity;
	float hysteresis;
	bool isRelayOn;
};

#define QUEUE_SIZE 1
#define QUEUE_DATA_TYPE SystemState

DHT_11 dht(DHT_PIN);

QueueHandle_t xStateQueue;
QueueHandle_t xControlQueue;

void monitor_task(void *param);
void sensor_task(void *param);
void control_task(void *param);

void lab_5_1_setup() {
	serial_setup();
	relay_setup(RELAY_PIN);
    dht.setup();

	xControlQueue = xQueueCreate(QUEUE_SIZE, sizeof(QUEUE_DATA_TYPE));
	xStateQueue = xQueueCreate(QUEUE_SIZE, sizeof(QUEUE_DATA_TYPE));

	if (xStateQueue == NULL || xControlQueue == NULL) {
		printf("Error creating queue\n");
		while (1)
			;
	}

	xTaskCreate(sensor_task, "SensorTask", 512, NULL, 1, NULL);
	xTaskCreate(control_task, "ControlTask", 128, NULL, 1, NULL);
	xTaskCreate(monitor_task, "MonitorTask", 256, NULL, 1, NULL);
}

void lab_5_1_loop() {}

void sensor_task(void *param) {
    SystemState state;
	for (;;) {
		state.humidity = dht.read_humidity();
        int pot_value = read_potentiometer(POT_PIN);
        state.hysteresis = 0.5f + ((float)pot_value / 1023.0f) * MAX_HYSTEREZIS;

		if (!isnan(state.humidity)) {
            xQueueOverwrite(xControlQueue, &state);
		} else {
            printf("DHT error\n");
        }
		vTaskDelay(pdMS_TO_TICKS(SENSOR_TASK_REQ));
	}
}

void control_task(void *param) {
    SystemState data;
    bool relayActive = false;

    for (;;) {
        if (xQueueReceive(xControlQueue, &data, portMAX_DELAY)) {
            if (data.humidity > (FIXED_SETPOINT + data.hysteresis)) {
                relay_NO_on(RELAY_PIN);
                relayActive = true;
            } 
            else if (data.humidity < (FIXED_SETPOINT - data.hysteresis)) {
                relay_NO_off(RELAY_PIN);
                relayActive = false;
            }

            data.isRelayOn = relayActive;
            xQueueOverwrite(xStateQueue, &data);
        }
        vTaskDelay(pdMS_TO_TICKS(CONTROL_TASK_REQ));
    }
}

void monitor_task(void *param) {
    QUEUE_DATA_TYPE state;
	for (;;) {
		if (xQueueReceive(xStateQueue, &state, portMAX_DELAY) == pdTRUE) {
            printf("\n[DATA] Hum: %.1f%% | SetPt: %.1f | Hyst: +/-%.1f | Fan: %s\n",
                   (double)state.humidity, 
                   (double)FIXED_SETPOINT, 
                   (double)state.hysteresis,
                   state.isRelayOn ? "ON" : "OFF");
		}
		vTaskDelay(pdMS_TO_TICKS(MONITOR_TASK_REQ));
	}
}

#endif
