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
#include "stdint.h"

// Noah libraries
#include "Motor.h"

typedef enum {
    IR_STOPPED,
    IR_READING,
    IR_DONE,
} IrMotionStatus;

/**
 * @brief Attatch this function to a DMA callback in order to process the data stored
 * in the DMA buffer.
 * i.e. DMA1_Channel3_IRQHandler()
 */
void irmotion_left_ir_handler();

/**
 * @brief Attatch this function to a DMA callback in order to process the data stored
 * in the DMA buffer.
 * i.e. DMA1_Channel3_IRQHandler()
 */
void irmotion_right_ir_handler();

/**
 * @brief Attatch this function to a timer overflow interrupt (the same as the timer you are using for
 * capture).
 * i.e. DMA1_Channel3_IRQHandler()
 */
void irmotion_timer_overflow_handler();

/**
 * @brief Initializes the library
 *
 * @param htimer Timer used for Capture mode (it must be initialized in CubeMx).
 * @param channelR Capture channel used for right sensor.
 * @param channelL Capture channel used for left sensor.
 */
void irmotion_init(TIM_HandleTypeDef *htimer, uint32_t channelR, uint32_t channelL);

/**
 * @brief Starts the measurement of the sensor
 *
 */
void irmotion_start_measurement(Motor motor);

/**
 * @brief Checks the status of the measurement.
 *
 * @return IrMotionStatus Status of the current measurement.
 */
IrMotionStatus irmotion_get_measurement_status(Motor motor);

/**
 * @brief Retrieves the current speed measured by the sensor.
 *
 * @return float Speed of the sensor in m/s.
 */
float irmotion_get_speed(Motor motor);

#endif  // IRMOTION_H