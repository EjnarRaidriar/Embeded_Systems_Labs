#include "lab_2_2.hpp"

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include "stdio_setups/serial_stdio/serail_stdio.hpp"
#include "led_v2/led_v2.hpp"
#include "button_v2/button_v2.hpp"

#define LED1 3
#define BUTTON1 4
#define LED2 5

#define QUEUE_SIZE 100

#define LED_TASK_REQ 10
#define TIME_LED_ON 1000
#define LED_BLINK_TASK_REQ_TIME 50
#define LED_BLINK_STATE_ON_TIME 300
#define LED_BLINK_STATE_OFF_TIME 500
#define MONITOR_TASK_REQ_TIME 200

void button_led_task(void *param);
void led_blink_task(void *param);
void monitor_task(void *param);

SemaphoreHandle_t button_semaphore;
QueueHandle_t dataQueue;
volatile int N = 0;

void lab_2_2_setup() {
  Serial.begin(9600);

  button_semaphore = xSemaphoreCreateBinary();
  dataQueue = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));

  serial_setup();

  if (button_semaphore == NULL || dataQueue == NULL) {
    printf("Error creating semaphore or queue\n");
    while(1);
  }

  xTaskCreate(
    button_led_task,
    "ButtonLed",
    128,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    led_blink_task,
    "BlinkingLed",
    128,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    monitor_task,
    "MonitorTask",
    128,
    NULL,
    1,
    NULL
  );
}

void lab_2_2_loop() {

}

void button_led_task(void *param) {
  led_v2_setup(LED1);
  button_v2_setup(BUTTON1);
  TickType_t last_wake_time;
  last_wake_time = xTaskGetTickCount();
  bool prev_button_state = HIGH;
  for (;;) {
    bool current_button_state = button_v2_read(BUTTON1);
    if (prev_button_state == HIGH && current_button_state == LOW) {
      xSemaphoreGive(button_semaphore);
      led_v2_turn_on(LED1);
      vTaskDelay(pdMS_TO_TICKS(TIME_LED_ON));
      led_v2_turn_off(LED1);
    }
    prev_button_state = current_button_state;
    vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(LED_TASK_REQ));
  }
}

void led_blink_task(void *param) {
  led_v2_setup(LED2);
  for (;;) {
    if (xSemaphoreTake(button_semaphore, portMAX_DELAY) == pdTRUE) {
      N += 1;
    }
    for (int i = 1; i <= N; i++) {
      uint8_t data = i;
      xQueueSend(dataQueue, &data, portMAX_DELAY);
      vTaskDelay(pdMS_TO_TICKS(LED_BLINK_TASK_REQ_TIME));
    }
    uint8_t end_marker = 0;
    xQueueSendToFront(dataQueue, &end_marker, portMAX_DELAY);
    for (int i = 0; i < N; i++) {
      led_v2_turn_on(LED2);
      vTaskDelay(pdMS_TO_TICKS(LED_BLINK_STATE_ON_TIME));
      led_v2_turn_off(LED2);
      vTaskDelay(pdMS_TO_TICKS(LED_BLINK_STATE_OFF_TIME));
    }
  }
}

void monitor_task(void *param) {
  serial_setup();
  uint8_t received_data;
  for (;;) {
    if (xQueueReceive(dataQueue, &received_data, 0) == pdTRUE) {
      if (received_data == 0) {
        printf("\n--- End of data sequence ---\n");
      }
      printf(" %d ", received_data);
    }
    vTaskDelay(pdMS_TO_TICKS(MONITOR_TASK_REQ_TIME));
  }
}