/**
 * @file Servo.h
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief This library manages the servo pin.
 * @version 0.1
 * @date 2019-12-31
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef SERVO_H
#define SERVO_H

// Standard libraries.
#include "stdint.h"

/**
 * @brief Initializes the servo
 *
 */
void servo_init(TIM_HandleTypeDef *htimer, uint32_t channel);

/**
 * @brief Moves the servo to the desired position.
 *
 * @param degree position of the servo in degrees.
 */
void servo_move(uint8_t degree);

/**
 * @brief Returns the position of the servo
 *
 * @return uint8_t Position of the servo in degrees.
 */
uint8_t servo_get_position();

#endif  // !SERVO_H