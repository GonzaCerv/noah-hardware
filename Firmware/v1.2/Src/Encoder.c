/**
 * @file Encoder.c
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Library for measuring speed of motors using as a sensor a quadrature hall encoder and
 *        the encoder moder for timer.
 * @version 0.1
 * @date 2020-03-15
 *
 * @copyright Copyright (c) 2020
 *
 */

// Cube Mx libraries
#include "tim.h"

// Noah libraries
#include "Encoder.h"

/// @brief stores the current and last count of the encoder.

bool isCountingUpL() {
    if ((ENCODER_TIMER_L.Instance->CR1 & 0b00010000)) {
        return false;
    }
    return true;
}

/// @brief stores the current and last count of the encoder.

bool isCountingUpR() {
    if ((ENCODER_TIMER_R.Instance->CR1 & 0b00010000)) {
        return false;
    }
    return true;
}
