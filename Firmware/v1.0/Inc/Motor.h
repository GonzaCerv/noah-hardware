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
#ifndef MOTOR_H
#define MOTOR_H

// Standard libraries
#include "math.h"
#include "stdlib.h"

// CubeMX libraries
#include "tim.h"

typedef enum { MOTOR_R, MOTOR_L } Motor;

/**
 * @brief Initializes the peripherals for controlling the robot.
 *
 * @param htimer    Timer that generates the PWM.
 * @param channelA  Channel of PWM for motor A.
 * @param channelB  Channel of PWM for motor B.
 */
void motor_init(TIM_HandleTypeDef *htimer, uint32_t channelA, uint32_t channelB);

/**
 * @brief
 *
 * @param motor Motor that whats to be moved.
 * @param duty speed of the motor.
 */
void motor_set_duty(Motor motor, int32_t duty);

/**
 * @brief Calculates the speed of the right motor.
 *
 * @param speed Speed that the robot should move in m/s
 * @param radius Radius of the movement in m.
 */
float motor_calculate_speed(Motor motor, float speed, float radius);

float motor_get_direction(Motor motor);

#endif  // !MOTOR_H
