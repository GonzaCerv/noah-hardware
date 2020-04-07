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

// Max speed that the motor can reach in m/s.

TIM_HandleTypeDef *hTimMot;
uint32_t           chanR;
uint32_t           chanL;
float channel_r_duty;
float channel_l_duty;

// Taken from https://www.waveshare.com/wiki/STM32CubeMX_Tutorial_Series:_PWM
void set_pwm_duty(TIM_HandleTypeDef *htimer, uint32_t channel, uint16_t value) {
    switch(channel){
        case TIM_CHANNEL_1:
            htimer->Instance->CCR1 = value;
            break;
        case TIM_CHANNEL_2:
            htimer->Instance->CCR2 = value;
            break;
        case TIM_CHANNEL_3:
            htimer->Instance->CCR3 = value;
            break;
        case TIM_CHANNEL_4:
            htimer->Instance->CCR4 = value;
            break;
        default:
        break;
    }
}

void motor_init(TIM_HandleTypeDef *htimer, uint32_t channelR, uint32_t channelL) {
    hTimMot = htimer;
    chanR = channelR;
    chanL = channelL;
    HAL_TIM_PWM_Start(htimer, channelR);
    HAL_TIM_PWM_Start(htimer, channelL);
    motor_set_duty(MOTOR_R, 0.0);
    motor_set_duty(MOTOR_L, 0.0);
    channel_l_duty =1.0;
    channel_r_duty = 1.0;
}

void motor_set_duty(Motor motor, int32_t duty) {
    switch (motor) {
        case MOTOR_R:
            if (duty > 0.0) {
                HAL_GPIO_WritePin(MOTOR_R_1_PORT, MOTOR_R_1_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(MOTOR_R_2_PORT, MOTOR_R_2_PIN, GPIO_PIN_RESET);
                channel_r_duty = 1.0;
                if (duty <= MAX_DUTY_PWM)
                    set_pwm_duty(hTimMot, chanR, duty);
                else
                    set_pwm_duty(hTimMot, chanR, MAX_DUTY_PWM);

            } else if (duty == 0.0) {
                HAL_GPIO_WritePin(MOTOR_R_1_PORT, MOTOR_R_1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(MOTOR_R_2_PORT, MOTOR_R_2_PIN, GPIO_PIN_RESET);
                set_pwm_duty(hTimMot, chanR, 0.0);
            } else {
                HAL_GPIO_WritePin(MOTOR_R_1_PORT, MOTOR_R_1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(MOTOR_R_2_PORT, MOTOR_R_2_PIN, GPIO_PIN_SET);
                channel_r_duty = -1.0;
                if (abs(duty) <= MAX_DUTY_PWM)
                    set_pwm_duty(hTimMot, chanR, abs(duty));
                else
                    set_pwm_duty(hTimMot, chanR, MAX_DUTY_PWM);
            }
            break;

        case MOTOR_L:
            if (duty > 0.0) {
                HAL_GPIO_WritePin(MOTOR_L_1_PORT, MOTOR_L_1_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(MOTOR_L_2_PORT, MOTOR_L_2_PIN, GPIO_PIN_RESET);
                channel_l_duty = 1.0;
                if (duty <= MAX_DUTY_PWM)
                    set_pwm_duty(hTimMot, chanR, duty);
                else
                    set_pwm_duty(hTimMot, chanL, MAX_DUTY_PWM);
            } else if (duty == 0.0) {
                HAL_GPIO_WritePin(MOTOR_L_1_PORT, MOTOR_L_1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(MOTOR_L_2_PORT, MOTOR_L_2_PIN, GPIO_PIN_RESET);
                set_pwm_duty(hTimMot, chanL, 0.0);
            } else {
                HAL_GPIO_WritePin(MOTOR_L_1_PORT, MOTOR_L_1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(MOTOR_L_2_PORT, MOTOR_L_2_PIN, GPIO_PIN_SET);
                channel_l_duty = -1.0;
                if (abs(duty) <= MAX_DUTY_PWM)
                    set_pwm_duty(hTimMot, chanL, abs(duty));
                else
                    set_pwm_duty(hTimMot, chanL, MAX_DUTY_PWM);
            }
            break;

        default:
            break;
    }
}

float motor_calculate_speed(Motor motor, float speed, float radius) {
    if (motor == MOTOR_R) {
        return speed * (radius + (LENGHT_WHEEL_TO_CENTER / 2));
    } else {
        return speed * (radius - (LENGHT_WHEEL_TO_CENTER / 2));
    }
}

float motor_get_direction(Motor motor) {
    if (motor == MOTOR_R) {
        return channel_r_duty;
    } else {
        return channel_l_duty;
    }
}