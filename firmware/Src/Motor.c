/**
 * @file Motor.c
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Module to manage Motors of Noah.
 * @version 0.1
 * @date 2019-12-27
 *
 * @copyright Copyright (c) 2019
 *
 */
// Standard libraries
#include "math.h"
// CubeMX libraries

// Noone libraries
#include "Config.h"
#include "Motor.h"

// Taken from https://www.waveshare.com/wiki/STM32CubeMX_Tutorial_Series:_PWM
void set_pwm_duty(TIM_HandleTypeDef *htimer, uint32_t channel, int32_t value) {
    uint32_t duty = (uint32_t)value;
    switch (channel) {
        case TIM_CHANNEL_1:
            htimer->Instance->CCR1 = duty;
            break;
        case TIM_CHANNEL_2:
            htimer->Instance->CCR2 = duty;
            break;
        case TIM_CHANNEL_3:
            htimer->Instance->CCR3 = duty;
            break;
        case TIM_CHANNEL_4:
            htimer->Instance->CCR4 = duty;
            break;
        default:
            break;
    }
}

void motor_init() {
    HAL_TIM_PWM_Start(&MOTOR_TIMER, MOTOR_CHANNEL_L);
    HAL_TIM_PWM_Start(&MOTOR_TIMER, MOTOR_CHANNEL_R);
    HAL_GPIO_WritePin(MOTOR_BRIDGE_EN_PORT, MOTOR_BRIDGE_EN_PIN, GPIO_PIN_SET);
    motor_set_duty(MOTOR_R, 0);
    motor_set_duty(MOTOR_L, 0);
}

void motor_set_duty(Motor motor, int32_t duty) {
    switch (motor) {
        case MOTOR_R:
            if (duty > 0) {
                HAL_GPIO_WritePin(MOTOR_R_1_PORT, MOTOR_R_1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(MOTOR_R_2_PORT, MOTOR_R_2_PIN, GPIO_PIN_SET);
                set_pwm_duty(&MOTOR_TIMER, MOTOR_CHANNEL_R, duty);

            } else if (duty == 0) {
                HAL_GPIO_WritePin(MOTOR_R_1_PORT, MOTOR_R_1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(MOTOR_R_2_PORT, MOTOR_R_2_PIN, GPIO_PIN_RESET);
                set_pwm_duty(&MOTOR_TIMER, MOTOR_CHANNEL_R, 0);
            } else {
                HAL_GPIO_WritePin(MOTOR_R_1_PORT, MOTOR_R_1_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(MOTOR_R_2_PORT, MOTOR_R_2_PIN, GPIO_PIN_RESET);
                set_pwm_duty(&MOTOR_TIMER, MOTOR_CHANNEL_R, abs(duty));
            }
            break;

        case MOTOR_L:
            if (duty > 0) {
                HAL_GPIO_WritePin(MOTOR_L_1_PORT, MOTOR_L_1_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(MOTOR_L_2_PORT, MOTOR_L_2_PIN, GPIO_PIN_RESET);
                set_pwm_duty(&MOTOR_TIMER, MOTOR_CHANNEL_L, duty);
            } else if (duty == 0) {
                HAL_GPIO_WritePin(MOTOR_L_1_PORT, MOTOR_L_1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(MOTOR_L_2_PORT, MOTOR_L_2_PIN, GPIO_PIN_RESET);
                set_pwm_duty(&MOTOR_TIMER, MOTOR_CHANNEL_L, 0);
            } else {
                HAL_GPIO_WritePin(MOTOR_L_1_PORT, MOTOR_L_1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(MOTOR_L_2_PORT, MOTOR_L_2_PIN, GPIO_PIN_SET);
                set_pwm_duty(&MOTOR_TIMER, MOTOR_CHANNEL_L, abs(duty));
            }
            break;

        default:
            break;
    }
}