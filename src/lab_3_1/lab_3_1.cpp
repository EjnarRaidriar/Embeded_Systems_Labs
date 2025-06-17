#include "lab_3_1.hpp"

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include "stdio_setups/serial_stdio/serail_stdio.hpp"
#include "joystick/joystick.hpp"

#define JOYSTICK_POS_X_PIN A1
#define JOYSTICK_POS_Y_PIN A0
#define JOYSTICK_BUTTON_PIN 2

#define MONITOR_TASK_REQ 500
#define JOYSTICK_TASK_REQ 100

#define QUEUE_SIZE 1
#define QUEUE_DATA_TYPE JoystickData

static QueueHandle_t dataQueue;

void joystick_task(void *param);
void monitor_task(void *param);

void lab_3_1_setup() {
  Serial.begin(9600);
  serial_setup();

  dataQueue = xQueueCreate(QUEUE_SIZE, sizeof(QUEUE_DATA_TYPE));

  if (dataQueue == NULL) {
    printf("Error creating queue\n");
    while(1);
  }

  xTaskCreate(joystick_task, "JoystickTask", 128, NULL, 1, NULL);
  xTaskCreate(monitor_task, "MonitorTask", 128, NULL, 1, NULL);
}

void lab_3_1_loop() {}

void joystick_task(void *param) {
  joystick_setup(JOYSTICK_BUTTON_PIN);
  TickType_t last_wake_time = xTaskGetTickCount();
  for (;;) {
    JoystickPosition pos = get_pos(JOYSTICK_POS_X_PIN, JOYSTICK_POS_Y_PIN);
    int switch_state = is_pressed(JOYSTICK_BUTTON_PIN);
    xQueueReset(dataQueue);
    JoystickData data = JoystickData{
      pos,
      switch_state
    };
    xQueueSend(dataQueue, &data, portMAX_DELAY);
    vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(JOYSTICK_TASK_REQ));
  }
}

void monitor_task(void *param) {
  serial_setup();
  JoystickData received_data;
  for (;;) {
    if (xQueueReceive(dataQueue, &received_data, 0) == pdTRUE) {
      printf("x: %d; y: %d;\n", received_data.pos.x, received_data.pos.y);
      if (received_data.button_state) {
        printf("Button is pressed\n");
      }
    }
    vTaskDelay(pdMS_TO_TICKS(MONITOR_TASK_REQ));
  }
}