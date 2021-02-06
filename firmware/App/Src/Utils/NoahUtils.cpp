/**
 * @file NoahUtils.c
 * @author Gonzalo Cervetti (cervetti.g@gmal.com)
 * @brief Various util functions for Noah.
 * @version 0.1
 * @date 2020-03-15
 *
 * @copyright Copyright (c) 2020
 *
 */

// CubeMx libraries
#include "main.h"

// Noah libraries
#include "Utils/NoahUtils.h"

namespace noah {
namespace utils {

void sortArray(int32_t *buffer, uint32_t buffer_size) {
  uint32_t i, j;
  for (i = 0; i < buffer_size; i++)  // Loop for ascending ordering
      {
    for (j = 0; j < buffer_size; j++)  // Loop for comparing other values
        {
      if (buffer[j] > buffer[i])  // Comparing other array elements
          {
        uint32_t tmp = buffer[i];  // Using temporary variable for storing last value
        buffer[i] = buffer[j];     // replacing value
        buffer[j] = tmp;           // storing last value
      }
    }
  }
}

float MeanCalc(int32_t *buffer, uint32_t buffer_size, uint32_t exclude_values) {
  uint32_t j, count = 0;
  int64_t tmp = 0;
  float result;

  for (j = exclude_values; j < (buffer_size - exclude_values); j++)  // Loop for comparing other values
      {
    tmp += buffer[j];
    ++count;
  }
  result = ((float) tmp / (float) count);
  return result;
}

float applyResolution(float val, float res) {
  if (val == 0.0f)
    return 0.00f;
  return (round((val + (res / 2)) / res) * res);
}

/**
 * @brief Configure one pin to output in runtime.
 * @param pin
 */
void dinamicSetPinToOutput(Pin pin) {
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
  GPIO_InitStruct.Pin = pin.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(pin.port, &GPIO_InitStruct);
}

/**
 * @brief Configure one pin to input in runtime.
 * @param pin
 */
void dinamicSetPinToInput(Pin pin) {
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
  GPIO_InitStruct.Pin = pin.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(pin.port, &GPIO_InitStruct);
}

}
}
