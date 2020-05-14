/**
 * @file Motor.c
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Module to manage Motors of Noah.
 * @version 0.1
 * @date 2019-12-27
 *
 * @copyright Copyright (c) 2019
 *
 */
#pragma once

// Standard libraries
#include "math.h"
#include "stdlib.h"

// CubeMX libraries
#include "tim.h"

typedef enum { MOTOR_R, MOTOR_L } Motor;

/**
 * @brief Initializes the peripherals for controlling the robot.
 *
 * @param channelR  Channel of PWM for motor A.
 * @param channelL  Channel of PWM for motor B.
 */
void motor_init();

/**
 * @brief
 *
 * @param motor Motor that whats to be moved.
 * @param duty speed of the motor.
 */
void motor_set_duty(Motor motor, int32_t duty);

