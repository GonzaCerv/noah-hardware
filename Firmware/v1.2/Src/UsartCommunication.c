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
#include "stdbool.h"
#include "stdint.h"

// CubeMx libraries
#include "cmsis_os.h"

// Noah libraries
#include "Config.h"
#include "Motor.h"
#include "UsartCommunication.h"

#define USART_BUFFER_SIZE 15

union {
    float   floatValue;
    uint8_t bytes[4];
} makeFloat;

uint8_t buffer[USART_BUFFER_SIZE];

extern osMutexId led_mutexHandle;

osStatus isLedMutexTaken;

extern float motorKP, motorKI, motorKD;
extern float targetSpeed_l, targetSpeed_r;

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
    if (buffer[0] != START_PACKAGE) {
        return false;
    }
    uint8_t param_count = ((buffer[1] & 0xF0) >> 4);
    // Increment 1 value to point to the end of the package and
    // skip the start package.
    ++param_count;
    ++param_count;
    if (buffer[param_count] != STOP_PACKAGE) {
        return false;
    }
    return true;
}

void usartComms_init() {
    HAL_UART_Receive_DMA(&UART_PORT, buffer, USART_BUFFER_SIZE);
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
            switch (buffer[1]) {
                case COMMAND_ECHO:
                    response[1] = COMMAND_ECHO;
                    response[2] = 0xAA;
                    response[3] = STOP_PACKAGE;
                    HAL_UART_Transmit(&UART_PORT, (uint8_t *)&response, 4, 10);
                    break;

                case COMMAND_MOVE:
                {
                    // Get the speed requested.
                    makeFloat.bytes[3] = buffer[2];
                    makeFloat.bytes[2] = buffer[3];
                    makeFloat.bytes[1] = buffer[4];
                    makeFloat.bytes[0] = buffer[5];
                    float new_speed_r = makeFloat.floatValue;

                    // Get the radius of the movement..
                    makeFloat.bytes[3] = buffer[6];
                    makeFloat.bytes[2] = buffer[7];
                    makeFloat.bytes[1] = buffer[8];
                    makeFloat.bytes[0] = buffer[9];
                    float new_speed_l = makeFloat.floatValue;

                    if(new_speed_r > PID_MAX_SPEED_MSEC){
                        new_speed_r = PID_MAX_SPEED_MSEC;
                    }
                    if(new_speed_l > PID_MAX_SPEED_MSEC){
                        new_speed_l = PID_MAX_SPEED_MSEC;
                    }
                    targetSpeed_l = applyResolutiom(new_speed_l,SPEED_RESOLUTION);
                    targetSpeed_r = applyResolutiom(new_speed_r,SPEED_RESOLUTION);
                }
                break;
                case COMMAND_MOTOR_KP:
                    // Get the speed requested.
                    makeFloat.bytes[3] = buffer[2];
                    makeFloat.bytes[2] = buffer[3];
                    makeFloat.bytes[1] = buffer[4];
                    makeFloat.bytes[0] = buffer[5];
                    motorKP = makeFloat.floatValue;
                    break;

                case COMMAND_MOTOR_KI:
                    // Get the speed requested.
                    makeFloat.bytes[3] = buffer[2];
                    makeFloat.bytes[2] = buffer[3];
                    makeFloat.bytes[1] = buffer[4];
                    makeFloat.bytes[0] = buffer[5];
                    motorKI = makeFloat.floatValue;
                    break;

                case COMMAND_MOTOR_KD:
                    // Get the speed requested.
                    makeFloat.bytes[3] = buffer[2];
                    makeFloat.bytes[2] = buffer[3];
                    makeFloat.bytes[1] = buffer[4];
                    makeFloat.bytes[0] = buffer[5];
                    motorKD = makeFloat.floatValue;
                    break;

                default:
                    response[1] = ERROR_COMMAND;
                    response[2] = STOP_PACKAGE;
                    HAL_UART_Transmit_DMA(&UART_PORT, (uint8_t *)&response, 3);
                    break;
            }
        }
        HAL_UART_Receive_DMA(&UART_PORT, buffer, USART_BUFFER_SIZE);
        ClearFlag_IDLE((USART_TypeDef *)&UART_PORT.Instance);
    }
}
