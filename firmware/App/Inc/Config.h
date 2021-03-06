/**
 * @file Config.h
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Configuration files for Noah Robot.
 * @version 0.1
 * @date 2020-03-15
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

// CubeMx libraries
#include "cmsis_os.h"
#include "main.h"
#include "tim.h"
#include "usart.h"

/**
 * @brief General Configurations
 *
 */
#define MESSAGE_QUEUE_BUFFER_SIZE 10

/**
 * @brief Driver configurations
 *
 */
#define MOTOR_TIMER htim4
#define MOTOR_CHANNEL_L TIM_CHANNEL_2
#define MOTOR_CHANNEL_R TIM_CHANNEL_3

#define PWM_MAX_VAL 719
#define PWM_MIN_VAL 90

/**
 * @brief Encoder configurations
 *
 */
#define ENCODER_UPDATE_RATE_MS 40.0f
#define STEPS_PER_TURN 918.0f
#define WHEEL_RADIUS 0.05f

#define ENCODER_TIMER_INT htim7
#define ENCODER_TIMER_L htim2
#define ENCODER_TIMER_R htim3

// The encoder library will truncate the measured speed to a multiple of this value
#define SPEED_RESOLUTION 0.01f

/**
 * @brief PID values
 *
 */
#define PID_DEFAULT_KP 300.0f
#define PID_DEFAULT_KI 700.0f
#define PID_DEFAULT_KD 20.0f
#define MAX_INTEGRATION_SUM 1.0f

/**
 * @brief Configuration for ROS comms
 *
 */
#define ROS_PORT huart1
#define ROS_TASK_UPDATE_RATE_MS 10

#define START_PACKAGE 0xA0
#define STOP_PACKAGE 0xB0
#define ERROR_COMMAND 0xFF

#define COMMAND_ECHO 0x00
#define COMMAND_OFF 0x02
#define COMMAND_LEFT_ENCODER_TICKS 0x20
#define COMMAND_RIGHT_ENCODER_TICKS 0x21
#define COMMAND_LEFT_SPEED 0x40
#define COMMAND_RIGHT_SPEED 0x41
#define COMMAND_MOTOR_KP 0xA5
#define COMMAND_MOTOR_KI 0xA6
#define COMMAND_MOTOR_KD 0xA7

/**
 * @brief Configuration for IMU
 *
 */
#define I2C_PORT hi2c1
