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
 * @brief Function to check if it is counting up.
 *
 * @return true when is couting up, false elsewhere.
 */
bool isCountingUpL();

/**
 * @brief Function to check if it is counting up.
 *
 * @return true when is couting up, false elsewhere.
 */
bool isCountingUpR();