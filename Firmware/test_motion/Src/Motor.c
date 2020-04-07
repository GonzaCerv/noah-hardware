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
#define MOTOR_MAX_SPEED (MOTOR_MAX_RPM_SPEED * GEARBOX_RELATION * 2 * M_PI * LAST_GEAR_RADIUS) / 60
#define MOTOR_MAX_SPEED_RAD (MOTOR_MAX_RPM_SPEED * GEARBOX_RELATION * 2 * M_PI) / 60

TIM_HandleTypeDef *hTimMot;
uint32_t           chanR;
uint32_t           chanL;

// Calculates the duty of the motor by giving a desired speed in m/s.
uint16_t calculate_duty(float speed) {
    if (speed > MOTOR_MAX_SPEED) return MAX_DUTY_PWM;
    float duty = speed * (MAX_DUTY_PWM / MOTOR_MAX_SPEED);
    if (duty < MIN_DUTY_PERCENTAGE)
        return 0;
    else
        return duty;
}

// Taken from https://www.waveshare.com/wiki/STM32CubeMX_Tutorial_Series:_PWM
void set_pwm_duty(TIM_HandleTypeDef *htimer, uint32_t channel, uint16_t value) {
    TIM_OC_InitTypeDef sConfigOC;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = value;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(htimer, &sConfigOC, channel);
    HAL_TIM_PWM_Start(htimer, channel);
}

void motor_init(TIM_HandleTypeDef *htimer, uint32_t channelR, uint32_t channelL) {
    hTimMot = htimer;
    chanR = channelR;
    chanL = channelL;
    HAL_TIM_PWM_Start(htimer, channelR);
    HAL_TIM_PWM_Start(htimer, channelL);
    motor_set_speed(MOTOR_R, 0.0);
    motor_set_speed(MOTOR_L, 0.0);
}

void motor_diff_move(float speed, float radius) {
    if (fabs(speed) <= (MOTOR_MAX_SPEED_RAD)) {
        float vr = motor_calculate_speed(MOTOR_R, speed, radius);
        float vl = motor_calculate_speed(MOTOR_L, speed, radius);
        motor_set_speed(MOTOR_R, vr);
        motor_set_speed(MOTOR_L, vl);
    }
}

void motor_set_speed(Motor motor, float speed) {
    uint16_t duty;
    switch (motor) {
        case MOTOR_R:
            if (speed > 0.0) {
                HAL_GPIO_WritePin(MOTOR_R_1_PORT, MOTOR_R_1_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(MOTOR_R_2_PORT, MOTOR_R_2_PIN, GPIO_PIN_RESET);
                duty = calculate_duty(speed);
                set_pwm_duty(hTimMot, chanR, duty);
            } else if (speed == 0.0) {
                HAL_GPIO_WritePin(MOTOR_R_1_PORT, MOTOR_R_1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(MOTOR_R_2_PORT, MOTOR_R_2_PIN, GPIO_PIN_RESET);
                set_pwm_duty(hTimMot, chanR, 0.0);
            } else {
                HAL_GPIO_WritePin(MOTOR_R_1_PORT, MOTOR_R_1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(MOTOR_R_2_PORT, MOTOR_R_2_PIN, GPIO_PIN_SET);
                duty = calculate_duty(fabs(speed));
                set_pwm_duty(hTimMot, chanR, duty);
            }
            break;

        case MOTOR_L:
            if (speed > 0.0) {
                HAL_GPIO_WritePin(MOTOR_L_1_PORT, MOTOR_L_1_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(MOTOR_L_2_PORT, MOTOR_L_2_PIN, GPIO_PIN_RESET);
                duty = calculate_duty(speed);
                set_pwm_duty(hTimMot, chanL, duty);
            } else if (speed == 0.0) {
                HAL_GPIO_WritePin(MOTOR_L_1_PORT, MOTOR_L_1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(MOTOR_L_2_PORT, MOTOR_L_2_PIN, GPIO_PIN_RESET);
                set_pwm_duty(hTimMot, chanL, 0.0);
            } else {
                HAL_GPIO_WritePin(MOTOR_L_1_PORT, MOTOR_L_1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(MOTOR_L_2_PORT, MOTOR_L_2_PIN, GPIO_PIN_SET);
                duty = calculate_duty(fabs(speed));
                set_pwm_duty(hTimMot, chanL, duty);
            }
            break;

        default:
            break;
    }
}

void motor_set_duty(Motor motor, int32_t duty) {
    switch (motor) {
        case MOTOR_R:
            if (duty > 0.0) {
                HAL_GPIO_WritePin(MOTOR_R_1_PORT, MOTOR_R_1_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(MOTOR_R_2_PORT, MOTOR_R_2_PIN, GPIO_PIN_RESET);
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

float motor_get_max_speed() { return MOTOR_MAX_SPEED; }