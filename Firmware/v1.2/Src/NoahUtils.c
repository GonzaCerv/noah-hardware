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

// Noah libraries
#include "NoahUtils.h"

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
    float    result;

    for (j = exclude_values; j < (buffer_size - exclude_values); j++)  // Loop for comparing other values
    {
        tmp += buffer[j];
        ++count;
    }
    result = ((float)tmp / (float)count);
    return result;
}

float applyResolutiom(float val, float res) {
    if (val == 0.0f) return 0.00f;
    return (round((val + (res / 2)) / res) * res);
}