#include "config.h"

#if APP_NAME == LAB_5_2

#ifndef LAB_5_2_CONFIG_HPP
#define LAB_5_2_CONFIG_HPP

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// PINs

#define PIN_MOTOR_ENCODER_C1 2
// #define PIN_MOTOR_ENCODER_C2 3

#define L298_ENA 5
#define L298_IN1 8
#define L298_IN2 9

#define PIN_POTENTIOMETER A0

// Config

#define POT_HIGH_THRESHOLD 1023
#define POT_LOW_THRESHOLD 0

#define PID_KP 0.8f
#define PID_KI 0.3f
#define PID_KD 0.03f

#define ENCODER_PULSES_PER_REV 330 // 30:1 for 11 pulses per motor shaft

#define SAMPLE_TIME_S 0.1f

#define DEFAULT_SETPOINT 100.0f // RPM target
#define SETPOINT_STEP 10.0f
#define SETPOINT_MIN 0.0f
#define SETPOINT_MAX 250.0f
#define SETPOINT_STEP_INTERVAL_MS 1500

#define PWM_MIN 0
#define PWM_MAX 255

// Requrance

// #define MONITOR_TASK_REQ 500
#define SPEED_TASK_REQ 100
#define INPUT_TASK_REQ 50
#define INPUT_TASK_SLEEP 300

// Queue

#define QUEUE_SIZE 5

extern QueueHandle_t rpm_queue;
extern QueueHandle_t pwm_queue;
extern QueueHandle_t monitor_queue;
extern QueueHandle_t setpoint_queue;

extern volatile long encoder_count;

void encoder_isr();

typedef struct {
	float setpoint;
	float rpm;
	int pwm;
} monitor_data_t;

#endif

#endif
