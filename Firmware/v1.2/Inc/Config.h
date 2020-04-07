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
 * @brief Motor driver configurations
 * 
 */
#define MOTOR_R_1_PORT  MOT_R_1_GPIO_Port
#define MOTOR_R_1_PIN   MOT_R_1_Pin
#define MOTOR_R_2_PORT  MOT_R_2_GPIO_Port
#define MOTOR_R_2_PIN   MOT_R_2_Pin

#define MOTOR_L_1_PORT  MOT_L_1_GPIO_Port
#define MOTOR_L_1_PIN   MOT_L_1_Pin
#define MOTOR_L_2_PORT  MOT_L_2_GPIO_Port
#define MOTOR_L_2_PIN   MOT_L_2_Pin

#define MOTOR_TIMER     htim4
#define MOTOR_CHANNEL_L TIM_CHANNEL_2
#define MOTOR_CHANNEL_R TIM_CHANNEL_3

/**
 * @brief Encoder configurations
 * 
 */
#define STEPS_PER_TURN  918.0f
#define ENCODER_UPDATE_INTERVAL_MS 20.0f 
#define ENCODER_TIMER_MAX_VAL   4000
#define ENCODER_BUFFER_SIZE 4
#define ENCODER_MEAN_DISCARD_COUNT 1
#define WHEEL_RADIUS 0.04f
#define SPEED_RESOLUTION  0.02f
#define SIGNAL_ENCODER_NEW_DATA 0x01

#define ENCODER_TIMER_L htim2
#define ENCODER_TIMER_R htim3

/**
 * @brief PID values
 * 
 */

#define PID_PARAM_KP 50.0f
#define PID_PARAM_KI 500.0f
#define PID_PARAM_KD 2.0f

#define MIN_DUTY_PWM    10
#define MAX_DUTY_PWM    71

#define MAX_INTEGRATION_SUM 1.0f
#define PID_UPDATE_RATE_MS (ENCODER_UPDATE_INTERVAL_MS * ENCODER_BUFFER_SIZE)

#define PID_MAX_SPEED_MSEC   0.5f

/**
 * @brief Kinematics
 * 
 */

#define LENGHT_WHEEL_TO_CENTER  0.07f

/**
 * @brief Configuration for USART
 *
 */
#define START_PACKAGE 0xA0
#define STOP_PACKAGE 0xB0
#define ERROR_COMMAND 0xFF

#define COMMAND_ECHO 0x00
#define COMMAND_BAT 0x01
#define COMMAND_OFF 0x02
#define COMMAND_MOVE 0x80
#define COMMAND_LED_AQUIRE 0x04
#define COMMAND_LED_RED_ON 0x05
#define COMMAND_LED_RED_OFF 0x06
#define COMMAND_LED_GREEN_ON 0x07
#define COMMAND_LED_GREEN_OFF 0x08
#define COMMAND_LED_RELEASE 0x09
#define COMMAND_SERVO_MOVE 0xA0
#define COMMAND_BUTTON_SHORT_PRESS 0xA1
#define COMMAND_BUTTON_LONG_PRESS 0xA2
#define COMMAND_DETECT_IR_FRONT 0xA3
#define COMMAND_DETECT_IR_BACK 0xA4
#define COMMAND_MOTOR_KP 0xA5
#define COMMAND_MOTOR_KI 0xA6
#define COMMAND_MOTOR_KD 0xA7

#define UART_COMMS_UPDATE_RATE_MS 50
#define UART_PORT   huart2

