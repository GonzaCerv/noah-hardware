/**
 * @file IRMotion.c
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief This library measures the speed of the motors by using an IR sensor and
 *  a slotted wheel.
 * @version 0.1
 * @date 2019-12-28
 *
 * @copyright Copyright (c) 2019
 *
 */
// Standard libraries
#include "math.h"
#include "stdbool.h"

// CubeMx libraries
#include "tim.h"

// Noah libraries
#include "Config.h"
#include "IRMotion.h"

const HAL_TIM_ActiveChannel channel_l_ = HAL_TIM_ACTIVE_CHANNEL_1;
const HAL_TIM_ActiveChannel channel_r_ = HAL_TIM_ACTIVE_CHANNEL_2;

uint16_t count_l_;
uint16_t count_r_;
uint16_t buffer_ir_l[2];
uint16_t buffer_ir_r[2];
float    ir_speed_r_;
float    ir_speed_l_;
uint16_t *ptr_l;
uint16_t *ptr_r;

/// @brief Timer handler.
TIM_HandleTypeDef *htim;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Channel == channel_l_) {
        if (count_l_ != 0) {
            *ptr_l = __HAL_TIM_GET_COUNTER(htim);
            ++ptr_l;
            --count_l_;
        }
    } else if (htim->Channel == channel_r_) {
        if (count_r_ != 0) {
            *ptr_r = __HAL_TIM_GET_COUNTER(htim);
            ++ptr_r;
            --count_r_;
        }
    }
}

void irmotion_init(TIM_HandleTypeDef *htimer) {
    htim = htimer;
    ir_speed_r_ = 0.0;
    ir_speed_l_ = 0.0;
}

void irmotion_start() {
    irmotion_reset();
    HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_2);
}

void irmotion_reset() {
    // Reset the buffers.
    ptr_l = &buffer_ir_l[1];
    *ptr_l = 0;
    --ptr_l;
    *ptr_l = 0;

    ptr_r = &buffer_ir_r[1];
    *ptr_r = 0;
    --ptr_r;
    *ptr_r = 0;

    // Reset the count and number of the values adquired.
    __HAL_TIM_SET_COUNTER(htim, 0);
    count_l_ = 2;
    count_r_ = 2;
}

void irmotion_stop() { HAL_TIM_IC_Stop_IT(htim, TIM_CHANNEL_1); }

float irmotion_get_speed(Motor motor) {
    if (motor == MOTOR_R) {
        return ir_speed_r_;
    }
    return ir_speed_l_;
}

void irmotion_update() {
    if (count_l_ == 0) {
        if (buffer_ir_l[1] > buffer_ir_l[0]) {
            uint16_t diff_l = buffer_ir_l[1] - buffer_ir_l[0];
            float t_1_turn_l = (float)(diff_l)*25.0f / 12000.0f;
            ir_speed_l_ = (2.0f * M_PI * 0.02f) / t_1_turn_l;
            ir_speed_l_ = (float)floor(ir_speed_l_ * 100.0f + 0.5f) / 100.0f;
        }
    } else {
        ir_speed_l_ = 0.0;
    }

    if (count_r_ == 0) {
        if (buffer_ir_r[1] > buffer_ir_r[0]) {
            uint16_t diff_r = buffer_ir_r[1] - buffer_ir_r[0];
            float t_1_turn_r = (float)(diff_r)*25.0f / 12000.0f;
            ir_speed_r_ = (2.0f * M_PI * 0.02f) / t_1_turn_r;
            ir_speed_r_ = (float)floor(ir_speed_r_ * 100.0f + 0.5f) / 100.0f;
        }
    } else {
        ir_speed_r_ = 0.0;
    }
}