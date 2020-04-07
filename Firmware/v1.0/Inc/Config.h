/**
 * @file Config.h
 * @author Gonzalo Cervettii (cervetti.g@gmail.com)
 * @brief Main configuration file for the whole project
 * @version 0.1
 * @date 2019-12-17
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef CONFIG_H
#define CONFIG_H

#include "main.h"

/**
 * @brief General configuration
 *
 */
#define RED_LED_PORT RED_LED_GPIO_Port
#define RED_LED_PIN RED_LED_Pin
#define GREEN_LED_PORT RED_LED_GPIO_Port
#define GREEN_LED_PIN RED_LED_Pin
#define LED_MUTEX_WAIT_TIME_MS 100

#define BUTTON_SIGNAL_READY 0x01
#define BATTERY_SIGNAL_READY 0x02
#define MOTION_CONTROL_SIGNAL_READY 0x03
#define START_TASK_SIGNAL_READY 0x04
#define SERVO_SIGNAL_READY 0x05
#define POWER_MGMT_SIGNAL_READY 0x06

#define IR_SENSOR_SIGNAL_READY 0x01

/**
 * @brief Configuration for BUTTON
 *
 */
#define BUTTON_LONG_PRESS_TICK 2000
#define BUTTON_SHORT_PRESS_TICK 50
#define BUTTON_UPDATE_RATE_MS 50

/**
 * @brief Configuration for Power Management
 *
 */
#define POWER_MGMT_UPDATE_RATE_MS 100
// #define ENABLE_3V_PORT EN_3V_GPIO_Port
// #define ENABLE_3V_PIN EN_3V_Pin
#define ENABLE_5V_PORT EN_5V_GPIO_Port
#define ENABLE_5V_PIN EN_5V_Pin
#define ENABLE_6V_PORT EN_6V_GPIO_Port
#define ENABLE_6V_PIN EN_6V_Pin
#define BQ24070_STAT1_PORT BQ24070_STAT1_GPIO_Port
#define BQ24070_STAT1_PIN BQ24070_STAT1_Pin
#define BQ24070_STAT2_PORT BQ24070_STAT2_GPIO_Port
#define BQ24070_STAT2_PIN BQ24070_STAT2_Pin
#define BQ24070_MODE_PORT BQ24070_MODE_GPIO_Port
#define BQ24070_MODE_PIN BQ24070_MODE_Pin
#define BUTTON_PORT BUTTON_GPIO_Port
#define BUTTON_PIN BUTTON_Pin

/**
 * @brief Configuration for Batery checking
 *
 */
#define BAT_CHECKING_UPDATE_RATE_MS 2000
#define BAT_OPERATIVE_THRESHOLD 3.0
#define ADC_RSUP 330000.0
#define ADC_RINF 330000.0
#define ADC_RES 4096.0

/**
 * @brief Configuration for motor management.
 *
 */
#define MOTOR_R_1_PORT MOTB1_GPIO_Port
#define MOTOR_R_1_PIN MOTB1_Pin
#define MOTOR_R_2_PORT MOTB2_GPIO_Port
#define MOTOR_R_2_PIN MOTB2_Pin

#define MOTOR_L_1_PORT MOTA1_GPIO_Port
#define MOTOR_L_1_PIN MOTA1_Pin
#define MOTOR_L_2_PORT MOTA2_GPIO_Port
#define MOTOR_L_2_PIN MOTA2_Pin

#define LENGHT_WHEEL_TO_CENTER 0.0726
#define MOTOR_MAX_RPM 1000.0
#define MOTOR_MAX_SPEED ((2 * M_PI * MOTOR_MAX_RPM) / 60.0)
/// @brief Max duty value. This value comes from the PWM calculation.
#define MAX_DUTY_PWM 71                    
#define MOTION_UPDATE_RATE 50

/**
 * @brief Configuration for IR motion.
 *
 */
#define SLOTS_IN_WHEEL 11356.0
#define CAPTURE_OVERFLOW_FREQUENCY 10.0
#define TIMER_TICK_FREQUENCY 1200.0
#define WHEEL_RADIUS 0.05
#define IRMOTION_L MOT_IRA_Pin
#define IRMOTION_R MOT_IRB_Pin
#define CAPTURE_UPDATE_RATE (1/CAPTURE_OVERFLOW_FREQUENCY)*1000.0

/**
 * @brief Configuration for motor PID
 *
 */
#define PID_UPDATE_RATE_MS 100.0  // Must match with CAPTURE_OVERFLOW_FREQUENCY
#define KP_VAL 0.0
#define KI_VAL 0.0
#define KD_VAL 0.0

/**
 * @brief Configuration for motor PID
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
#define UART_COMMS_UPDATE_RATE 50

/**
 * @brief Configuration for Servo
 *
 */
#define START_POS_DEGREE 90
#define MIN_DUTY_POS 181
#define MAX_DUTY_POS 361

/**
 * @brief Configuration for Servo
 *
 */
#define FLOOR_IRF_PORT FLOOR_IRF_GPIO_Port
#define FLOOR_IRF_PIN FLOOR_IRF_Pin
#define FLOOR_IRB_PORT FLOOR_IRB_GPIO_Port
#define FLOOR_IRB_PIN FLOOR_IRB_Pin
#define FLOOR_IR_UPDATE_RATE 100

#endif  // ! CONFIG_H
