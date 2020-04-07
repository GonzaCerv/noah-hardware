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

/// @brief Timer handler.
TIM_HandleTypeDef *htim;

uint32_t chnlL;
uint32_t chnlR;

/// @brief Buffer that stores each capture value.
uint16_t bufferL[2];
uint16_t bufferR[2];

bool lHasOverflowed;
bool rHasOverflowed;
bool measureLSuccessful;
bool measureRSuccessful;
bool measureLStarted;
bool measureRStarted;

float irLeftSpeed;
float irRightSpeed;

void irmotion_left_ir_handler() {
    if (!lHasOverflowed) {
        // To calculate speed, calculate how many RPMs has measured the Capture module
        //(Frequency * 60). Then multiply it by the gearbox relationship (in order to get the RPMs at
        // Wheel stage) and finally, multiply it by the circumference of the wheel.
        irLeftSpeed =
            (CAPTURE_TICK_FREQUENCY / (bufferL[1] - bufferL[0])) * 60 * GEARBOX_RELATION * 2 * M_PI * LAST_GEAR_RADIUS;
        measureLSuccessful = true;
    }
}

void irmotion_right_ir_handler() {
    if (!rHasOverflowed) {
        irLeftSpeed =
            (CAPTURE_TICK_FREQUENCY / (bufferL[1] - bufferL[0])) * 60 * GEARBOX_RELATION * 2 * M_PI * LAST_GEAR_RADIUS;
        measureLSuccessful = false;
    }
}

void irmotion_timer_overflow_handler() {
    if (!measureLSuccessful) {
        irLeftSpeed = 0.0;
        lHasOverflowed = true;
        measureLSuccessful = true;
    }
    if (!measureRSuccessful) {
        irRightSpeed = 0.0;
        rHasOverflowed = true;
        measureRSuccessful = true;
    }

    HAL_TIM_IC_Stop_DMA(htim, chnlL);
    HAL_TIM_IC_Stop_DMA(htim, chnlR);
}

void irmotion_init(TIM_HandleTypeDef *htimer, uint32_t channelR, uint32_t channelL) {
    htim = htimer;
    chnlL = channelL;
    chnlR = channelR;
    lHasOverflowed = false;
    rHasOverflowed = false;
    measureLSuccessful = false;
    measureRSuccessful = false;
    measureLStarted = false;
    measureRStarted = false;
    irLeftSpeed = 0.0;
    irRightSpeed = 0.0;
}

void irmotion_start_measurement(Motor motor) {
    if (motor == MOTOR_L) {
        HAL_TIM_IC_Start_DMA(htim, chnlL, (uint32_t *)bufferL, 2);
        lHasOverflowed = false;
        measureLSuccessful = false;
        measureLStarted = true;
    } else {
        HAL_TIM_IC_Start_DMA(htim, chnlR, (uint32_t *)bufferR, 2);
        rHasOverflowed = false;
        measureRSuccessful = false;
        measureRStarted = true;
    }
}

IrMotionStatus irmotion_get_measurement_status(Motor motor) {
    if (motor == MOTOR_L) {
        if (!measureLStarted) return IR_STOPPED;
        if (measureLSuccessful)
            return IR_DONE;
        else
            return IR_READING;
    } else {
        if (!measureRStarted) return IR_STOPPED;
        if (measureRSuccessful)
            return IR_DONE;
        else
            return IR_READING;
    }
}

float irmotion_get_speed(Motor motor) {
    if (motor == MOTOR_L) {
        if (IR_DONE == irmotion_get_measurement_status(MOTOR_L)) {
            measureLStarted = false;
            measureLSuccessful = false;
            lHasOverflowed = false;
            return irLeftSpeed;
        } else {
            return 0.0;
        }
    } else {
        if (IR_DONE == irmotion_get_measurement_status(MOTOR_R)) {
            measureRStarted = false;
            measureRSuccessful = false;
            rHasOverflowed = false;
            return irRightSpeed;
        } else {
            return 0.0;
        }
    }
}
