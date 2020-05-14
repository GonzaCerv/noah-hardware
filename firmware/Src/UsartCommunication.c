/**
 * @file UsartCommunication.c
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Manages the communication with the raspberry
 * @version 0.1
 * @date 2019-12-31
 *
 * @copyright Copyright (c) 2019
 *
 */

// Standard libraries
#include "math.h"
#include "stdint.h"

// CubeMx libraries
#include "cmsis_os.h"

// Noah libraries
#include "Config.h"
#include "Motor.h"
#include "UsartCommunication.h"

#define USART_BUFFER_SIZE 15

union {
    float floatValue;
    uint8_t bytes[4];
} makeFloat;

uint8_t buffer_[USART_BUFFER_SIZE];

extern float motorKP, motorKI, motorKD;
extern int16_t current_ticks_l, current_ticks_r;
float mot_target_speed_l, mot_target_speed_r;
bool turn_off_sequence_;

/**
 * @brief Enables the interrupt for RX idle line.
 *
 * @param USARTx
 */
void EnableIT_IDLE(USART_TypeDef *USARTx) { SET_BIT(USARTx->CR1, USART_CR1_IDLEIE); }

/**
 * @brief Checks if the IDLE interrupt has been fired.
 *
 * @param USARTx
 * @return bool true when interrupt was fired, false when not.
 */
bool IsActiveFlag_IDLE(USART_TypeDef *USARTx) {
    if (READ_BIT(USARTx->SR, USART_SR_IDLE) == (USART_SR_IDLE)) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Clears the flag of IDLE line.
 *
 * @param USARTx
 */
void ClearFlag_IDLE(USART_TypeDef *USARTx) {
    __IO uint32_t tmpreg;
    tmpreg = USARTx->SR;
    (void)tmpreg;
    tmpreg = USARTx->DR;
    (void)tmpreg;
}

/**
 * @brief Checks the format of the incomming package
 *
 * @return true when package is valid.
 */
bool check_package_format() {
    if (buffer_[0] != START_PACKAGE) {
        return false;
    }
    uint8_t param_count = ((buffer_[1] & 0xF0) >> 4);
    // Increment 1 value to point to the end of the package and
    // skip the start package.
    ++param_count;
    ++param_count;
    if (buffer_[param_count] != STOP_PACKAGE) {
        return false;
    }
    return true;
}

void usartComms_init() {
    turn_off_sequence_ = false;
    mot_target_speed_l = 0.0f;
    mot_target_speed_r = 0.0f;
    HAL_UART_Receive_DMA(&UART_PORT, buffer_, USART_BUFFER_SIZE);
    EnableIT_IDLE(UART_PORT.Instance);
}

void usartComms_update() {
    uint8_t response[10];

    if (IsActiveFlag_IDLE(UART_PORT.Instance)) {
        // Stop DMA so package can be processed.
        HAL_UART_DMAStop(&UART_PORT);
        // If the package has start and stop identifier.
        if (check_package_format()) {
            response[0] = START_PACKAGE;
            // Process the data.
            switch (buffer_[1]) {
                case COMMAND_ECHO:
                    response[1] = COMMAND_ECHO;
                    response[2] = 0xAA;
                    response[3] = STOP_PACKAGE;
                    HAL_UART_Transmit(&UART_PORT, (uint8_t *)&response, 4, 10);
                    break;

                case COMMAND_LEFT_SPEED: {
                    // Get the speed requested.
                    makeFloat.bytes[3] = buffer_[2];
                    makeFloat.bytes[2] = buffer_[3];
                    makeFloat.bytes[1] = buffer_[4];
                    makeFloat.bytes[0] = buffer_[5];
                    float new_speed_l = makeFloat.floatValue;

                    if (new_speed_l > MAX_SPEED_METER_SEC) {
                        new_speed_l = MAX_SPEED_METER_SEC;
                    } else if (fabs(new_speed_l) > MAX_SPEED_METER_SEC) {
                        new_speed_l = -MAX_SPEED_METER_SEC;
                    }
                    mot_target_speed_l = applyResolutiom(new_speed_l,SPEED_RESOLUTION);
                } break;

                case COMMAND_RIGHT_SPEED: {
                    // Get the speed requested.
                    makeFloat.bytes[3] = buffer_[2];
                    makeFloat.bytes[2] = buffer_[3];
                    makeFloat.bytes[1] = buffer_[4];
                    makeFloat.bytes[0] = buffer_[5];
                    float new_speed_r = makeFloat.floatValue;

                    if (new_speed_r > MAX_SPEED_METER_SEC) {
                        new_speed_r = MAX_SPEED_METER_SEC;
                    } else if (fabs(new_speed_r) > MAX_SPEED_METER_SEC) {
                        new_speed_r = -MAX_SPEED_METER_SEC;
                    }
                    mot_target_speed_r = applyResolutiom(new_speed_r,SPEED_RESOLUTION);
                } break;

                case COMMAND_LEFT_ENCODER_TICKS: {
                    uint16_t current_ticks = current_ticks_l;
                    response[1] = COMMAND_LEFT_ENCODER_TICKS;
                    response[2] = (uint8_t)((current_ticks >> 8) & 0xFF);
                    response[3] = (uint8_t)(current_ticks & 0xFF);
                    response[4] = STOP_PACKAGE;
                    HAL_UART_Transmit(&UART_PORT, (uint8_t *)&response, 5, 10);
                } break;

                case COMMAND_RIGHT_ENCODER_TICKS: {
                    uint16_t current_ticks = current_ticks_r;
                    response[1] = COMMAND_RIGHT_ENCODER_TICKS;
                    response[2] = (uint8_t)((current_ticks >> 8) & 0xFF);
                    response[3] = (uint8_t)(current_ticks & 0xFF);
                    response[4] = STOP_PACKAGE;
                    HAL_UART_Transmit(&UART_PORT, (uint8_t *)&response, 5, 10);
                } break;

                case COMMAND_OFF:
                    turn_off_sequence_ = true;
                    break;
                default:
                    response[1] = ERROR_COMMAND;
                    response[2] = STOP_PACKAGE;
                    HAL_UART_Transmit(&UART_PORT, (uint8_t *)&response, 3, 10);
                    break;
            }
        }
        HAL_UART_Receive_DMA(&UART_PORT, buffer_, USART_BUFFER_SIZE);
        ClearFlag_IDLE((USART_TypeDef *)&UART_PORT.Instance);
    }
}

void usartComms_publish_off() {
    buffer_[0] = START_PACKAGE;
    buffer_[1] = COMMAND_OFF;
    buffer_[2] = STOP_PACKAGE;
    HAL_UART_Transmit(&UART_PORT, (uint8_t *)&buffer_, 3, 10);
}

float usartComms_get_target_speed_l() { return mot_target_speed_l; }

float usartComms_get_target_speed_r() { return mot_target_speed_r; }

bool usartComms_is_turn_off_requested() { return turn_off_sequence_; }
