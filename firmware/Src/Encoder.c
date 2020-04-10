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
// Standard libraries

// Cube Mx libraries
#include "tim.h"

// Noah libraries
#include "Encoder.h"

inline int min(int a, int b) {
    if (a > b) return b;
    return a;
}

inline int max(int a, int b) {
    if (a < b) return b;
    return a;
}

void encoder_init(){
    HAL_TIM_Encoder_Start_IT(&ENCODER_TIMER_L, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start_IT(&ENCODER_TIMER_R, TIM_CHANNEL_ALL);
    HAL_TIM_Base_Start_IT(&ENCODER_TIMER_INT);
}

int16_t encoder_calculate_diff(int16_t previous, int16_t current) {
    int16_t result;
    if (has_wrapped(previous, current)) {
        result = previous + current;
        // Return negative value if the rotation is clockwise.
        if(current > previous) {
            result = -result;
        }
    }else{
        result = max(previous, current) - min(previous, current);
        // Return negative value if the rotation is clockwise.
        if(current < previous) {
            result = -result;
        }
    }
    return result;
}

bool has_wrapped(int16_t previous, int16_t current) {
    if ((min(previous, current) < (-13107)) && (max(previous, current) > 13106)) {
        return true;
    }
    return false;
}

/// @brief stores the current and last count of the encoder.
bool encoder_is_counting_up_l() {
    if ((ENCODER_TIMER_L.Instance->CR1 & 0b00010000)) {
        return false;
    }
    return true;
}

/// @brief stores the current and last count of the encoder.
bool encoder_is_counting_up_r() {
    if ((ENCODER_TIMER_R.Instance->CR1 & 0b00010000)) {
        return false;
    }
    return true;
}
