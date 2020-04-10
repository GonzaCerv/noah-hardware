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

#include "main.h"

// CubeMx libraries
#include "tim.h"
#include "usart.h"

/**
 * @brief pinout driver configurations
 *
 */
#define MOTOR_R_1_PORT MOT_R1_GPIO_Port
#define MOTOR_R_1_PIN MOT_R1_Pin
#define MOTOR_R_2_PORT MOT_R2_GPIO_Port
#define MOTOR_R_2_PIN MOT_R2_Pin

#define MOTOR_L_1_PORT MOT_L1_GPIO_Port
#define MOTOR_L_1_PIN MOT_L1_Pin
#define MOTOR_L_2_PORT MOT_L2_GPIO_Port
#define MOTOR_L_2_PIN MOT_L2_Pin

#define MOTOR_BRIDGE_EN_PORT STAND_BY_GPIO_Port
#define MOTOR_BRIDGE_EN_PIN STAND_BY_Pin

#define MOTOR_TIMER htim4
#define MOTOR_CHANNEL_L TIM_CHANNEL_2
#define MOTOR_CHANNEL_R TIM_CHANNEL_3

#define SMPS_OFF_PORT SMPS_EN_GPIO_Port
#define SMPS_OFF_PIN SMPS_EN_Pin

#define PWM_MAX_VAL 719.0f
#define PWM_MIN_VAL 90.0f

/**
 * @brief Encoder configurations
 *
 */
#define TICK_MAX_VAL 32767
#define TICK_MIN_VAL -32768
#define TICK_HIGH_WRAP ((0.3 * (TICK_MAX_VAL - TICK_MIN_VAL)) + TICK_MIN_VAL)
#define TICK_LOW_WRAP ((0.7 * (TICK_MAX_VAL - TICK_MIN_VAL)) + TICK_MIN_VAL)

// This value has to be the same as the update rate of the interrupt that
// reads the values of the timers connected to the encoder.
#define MAX_SPEED_METER_SEC 0.6f
#define SPEED_RESOLUTION  0.02f

#define ENCODER_TIMER_INT htim7
#define ENCODER_TIMER_L htim2
#define ENCODER_TIMER_R htim3
#define ENCODER_UPDATE_INTERVAL_MS 40

/**
 * @brief PID values
 *
 */
#define MAX_INTEGRATION_SUM 1.0f
#define PID_DEFAULT_KP 150.0f
#define PID_DEFAULT_KI 5500.0f
#define PID_DEFAULT_KD 1.0f
#define STEPS_PER_TURN 918.0f
#define WHEEL_RADIUS 0.041f

/**
 * @brief OFF values
 *
 */
#define OFF_WAIT_TIME_MS 4000

/**
 * @brief Configuration for USART
 *
 */
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

#define UART_COMMS_UPDATE_RATE_MS 10
#define UART_PORT huart2
