#include "lab_3_2.hpp"
#include "filter/filter.hpp"
#if APP_NAME == LAB_3_2

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include "joystick/joystick.hpp"
#include "stdio_setups/serial_stdio/serail_stdio.hpp"

#define JOYSTICK_POS_X_PIN A1
#define JOYSTICK_POS_Y_PIN A0
#define JOYSTICK_BUTTON_PIN 2

#define SIGNAL_MIN 0
#define SIGNAL_MAX 1023
#define mV_MIN 0
#define mV_MAX 5000

#define MONITOR_TASK_REQ 200
#define JOYSTICK_TASK_REQ 100

int weights[BUFFER_SIZE] = {50, 35, 25, 15, 10};

struct FilteredJoystickData {
  long x;
  long y;
  int button_state;
};

#define QUEUE_SIZE 1
#define QUEUE_DATA_TYPE FilteredJoystickData

static QueueHandle_t dataQueue;

Filter filter_x_1;
Filter filter_y_1;
Filter filter_x_2;
Filter filter_y_2;

void joystick_task(void *param);
void monitor_task(void *param);

void lab_3_2_setup() {
  Serial.begin(9600);
  serial_setup();

  dataQueue = xQueueCreate(QUEUE_SIZE, sizeof(QUEUE_DATA_TYPE));

  if (dataQueue == NULL) {
    printf("Error creating queue\n");
    while (1)
      ;
  }

  xTaskCreate(joystick_task, "JoystickTask", 128, NULL, 1, NULL);
  xTaskCreate(monitor_task, "MonitorTask", 128, NULL, 1, NULL);
}
void lab_3_2_loop() {}

void joystick_task(void *param) {
  joystick_setup(JOYSTICK_BUTTON_PIN);
  TickType_t last_wake_time = xTaskGetTickCount();
  for (;;) {
    JoystickPosition pos = get_pos(JOYSTICK_POS_X_PIN, JOYSTICK_POS_Y_PIN);
    int switch_state = is_pressed(JOYSTICK_BUTTON_PIN);

    long x = map(pos.x, SIGNAL_MIN, SIGNAL_MAX, mV_MIN, mV_MAX);
    long y = map(pos.y, SIGNAL_MIN, SIGNAL_MAX, mV_MIN, mV_MAX);

    filter_x_1.add(x);
    filter_y_1.add(y);

    if (filter_x_1.ready())
      x = filter_x_1.median();
    if (filter_y_1.ready())
      y = filter_y_1.median();

    filter_x_2.add(x);
    filter_y_2.add(y);

    if (filter_x_2.ready())
      x = filter_x_2.wighted_average(weights);
    if (filter_y_2.ready())
      y = filter_y_2.wighted_average(weights);

    FilteredJoystickData data = FilteredJoystickData{x, y, switch_state};
    xQueueOverwrite(dataQueue, &data);
    vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(JOYSTICK_TASK_REQ));
  }
}

void monitor_task(void *param) {
  serial_setup();
  FilteredJoystickData received_data;
  for (;;) {
    if (xQueueReceive(dataQueue, &received_data, 0) == pdTRUE) {
      printf("x: %ld mV; y: %ld mV;\n", received_data.x, received_data.y);
      if (received_data.button_state) {
        printf("Button is pressed\n");
      }
    }
    vTaskDelay(pdMS_TO_TICKS(MONITOR_TASK_REQ));
  }
}

#endif
