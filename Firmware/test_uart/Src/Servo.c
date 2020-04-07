/**
 * @file Servo.c
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief This library manages the servo pin.
 * @version 0.1
 * @date 2019-12-31
 *
 * @copyright Copyright (c) 2019
 *
 */
// CubeMx libraries
#include "tim.h"

// Noah libraries
#include "Config.h"
#include "Servo.h"

TIM_HandleTypeDef *htim;
uint32_t           chan;
uint8_t            currentPos;

// Taken from https://www.waveshare.com/wiki/STM32CubeMX_Tutorial_Series:_PWM
void set_servo_duty(TIM_HandleTypeDef *htimer, uint32_t channel, uint16_t value) {
    TIM_OC_InitTypeDef sConfigOC;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = value;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(htimer, &sConfigOC, channel);
    HAL_TIM_PWM_Start(htimer, channel);
}

uint16_t calc_duty(uint8_t degree) { return (MAX_DUTY_POS - MIN_DUTY_POS) * (degree / 180) + MIN_DUTY_POS; }

/**
 * @brief Initializes the servo
 *
 */
void servo_init(TIM_HandleTypeDef *htimer, uint32_t channel) {
    htim = htimer;
    chan = channel;
    currentPos = calc_duty(START_POS_DEGREE);
    HAL_TIM_PWM_Start(htim, chan);
    set_servo_duty(htim, chan, currentPos);
}

/**
 * @brief Moves the servo to the desired position.
 *
 * @param degree
 */
void servo_move(uint8_t degree) {
    currentPos = calc_duty(degree);
    set_servo_duty(htim, chan, currentPos);
}

/**
 * @brief Returns the position of the servo
 *
 * @return uint8_t Position of the servo.
 */
uint8_t servo_get_position() { return currentPos; }