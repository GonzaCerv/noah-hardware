/**
 * @file IRMotion.h
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief This library measures the speed of the motors by using an IR sensor and
 *  a slotted wheel.
 * @version 0.1
 * @date 2019-12-28
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef IRMOTION_H
#define IRMOTION_H

// Standard libraries
#include "stdbool.h"
#include "stdint.h"

// Noah libraries
#include "Config.h"
#include "Motor.h"

typedef enum {
    IR_STOPPED,
    IR_READING,
    IR_DONE,
} IrMotionStatus;

/**
 * @brief Attatch this function to a timer overflow interrupt (the same as the timer you are using for
 * capture).
 * i.e. DMA1_Channel3_IRQHandler()
 */
void irmotion_timer_overflow_handler();

/**
 * @brief Sets the values to start measuring.
 * 
 * @param htimer 
 * @param channel_l Timer channel to measure the left IR sensor.
 * @param channel_r Timer channel to measure the right IR sensor.
 */
void irmotion_init(TIM_HandleTypeDef *htimer);

/**
 * @brief Starts the measurement of the sensors
 *
 */
void irmotion_start();

/**
 * @brief Resets the values to start measuring again.
 * 
 */
void irmotion_reset();

/**
 * @brief Stops the measurement of the sensors
 *
 */
void irmotion_stop();

/**
 * @brief Calculates the new values for the current speed of each sensor.
 * 
 */
void irmotion_update();

/**
 * @brief Retrieves the current speed measured by the sensor.
 *
 * @return float Speed of the sensor in m/s.
 */
float irmotion_get_speed(Motor motor);

#endif  // IRMOTION_H