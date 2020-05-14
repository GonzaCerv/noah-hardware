/**
 * @file Encoder.h
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Library for measuring speed of motors using as a sensor a quadrature hall encoder and
 *        the encoder moder for timer.
 * @version 0.1
 * @date 2020-03-15
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

// C libraries.
#include "stdbool.h"
#include "stdint.h"

// Noah libraries
#include "Config.h"
#include "math.h"


/**
 * @brief Initializes all the peripherals related to the encoders.
 * 
 */
void encoder_init();

/**
 * @brief calculates the encoder difference between different
 *        measurements of the ticks. It takes into account
 *        when the count overflows.
 *
 * @param previous current absolute tick count
 * @param current  previous absolute tick count
 * @return int16_t amount of ticks between both tick counts. The
 *                 sign indicates if the rotation is positive or negative.
 */
int16_t encoder_calculate_diff(int16_t previous, int16_t current);

/**
 * @brief Checks if there was overflow between the current and the
 *        previous value of the ticks. In order to check this,
 *        check if the prevous value is above tick_high_wrap and the
 *        current value es below tick_low_wrap or viceversa
 *
 * @param previous current absolute tick count
 * @param current previous absolute tick count
 * @return true
 * @return false
 */
bool has_wrapped(int16_t previous, int16_t current);

/**
 * @brief Function to check if it is counting up.
 *
 * @return true when is couting up, false elsewhere.
 */
bool encoder_is_counting_up_l();

/**
 * @brief Function to check if it is counting up.
 *
 * @return true when is couting up, false elsewhere.
 */
bool encoder_is_counting_up_r();