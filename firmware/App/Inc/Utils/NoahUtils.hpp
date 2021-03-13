/**
 * @file NoahUtils.hpp
 * @author Gonzalo Cervetti (cervetti.g@gmal.com)
 * @brief Various utilities for Noahproject.
 * @version 0.1
 * @date 2020-03-15
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

// C libraries
#include "math.h"
#include "stdint.h"

// CubeMx libraries
#include "stm32f407xx.h"
#include "tim.h"

namespace noah {
namespace utils {

// @brief Pin structure
typedef struct {
  GPIO_TypeDef *port;
  uint16_t pin;
} Pin;

// @brief Timer channel structure
typedef struct {
  TIM_HandleTypeDef *timer;
  uint32_t channel;
} MotorChannel;

/**
 * @brief Sort an array of values.
 *
 * @param buffer array to be sorted.
 * @param buffer_size  size of the array.
 */
void sortArray(int32_t *buffer, uint32_t buffer_size);

/**
 * @brief Calculate the mathematical mean of an array.
 *
 * @param buffer array of values to read
 * @param buffer_size size of the buffer
 * @param exclude_values values to be excluded from the mean calculation.
 * @return float
 */
float MeanCalc(int32_t *buffer, uint32_t buffer_size, uint32_t exclude_values);

/**
 * @brief returns val rounded to a value multiple of res.
 *
 * @param val
 * @param res
 * @return float
 */
float applyResolution(float val, float res);

/**
 * @brief Configure one pin to output in runtime.
 * @param pin
 */
void dinamicSetPinToOutput(Pin pin);

/**
 * @brief Configure one pin to input in runtime.
 * @param pin
 */
void dinamicSetPinToInput(Pin pin);

/**
 * @brief Sets X between A and B
 * @tparam T Type of the value to constrain.
 * @param x value to constrain.
 * @param a min value.
 * @param b max value.
 * @return x constrained.
 */
template<class T>
const T& constrain(const T &x, const T &a, const T &b) {
  if (x < a) {
    return a;
  } else if (b < x) {
    return b;
  } else
    return x;
}

}
}
