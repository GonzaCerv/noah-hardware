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
#include "usart.h"

// Noah libraries
#include "Battery.h"
#include "Button.h"
#include "Config.h"
#include "IRFloor.h"
#include "Motor.h"
#include "PowerManagement.h"
#include "Servo.h"
#include "UsartCommunication.h"

#define USART_BUFFER_SIZE 15

union {
    float   floatValue;
    uint8_t bytes[4];
} makeFloat;

UART_HandleTypeDef *huartx;

uint8_t buffer[USART_BUFFER_SIZE];

extern osMutexId led_mutexHandle;

osStatus isLedMutexTaken;

extern float motorKP, motorKI, motorKD;
extern float targetSpeedL, targetSpeedR;

bool transmition_sent;

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

bool check_package_format() {
    uint16_t count;
    bool     isStartIdentifierPresent = false;
    bool     isStopIdentifierPresent = false;
    for (count = 0; count < USART_BUFFER_SIZE; ++count) {
        if (buffer[count] == START_PACKAGE) {
            isStartIdentifierPresent = true;
        } else {
            if (buffer[count] == STOP_PACKAGE) isStopIdentifierPresent = true;
        }
        if (isStartIdentifierPresent && isStopIdentifierPresent) return true;
    }
    return false;
}

/**
 * @brief Sends the command to turn off the Raspberry.
 *
 * @return true everything goes ok.
 * @return false something happened.
 */
bool send_off_command() {
    uint8_t response[] = {START_PACKAGE, COMMAND_OFF, STOP_PACKAGE};
    HAL_UART_Transmit(huartx, (uint8_t *)&response, sizeof(response), UART_TIMEOUT_MS);
    // Let the device turn off.
    osDelay(3000);
    return true;
}

/**
 * @brief This function is a callback that sends a message
 *  when a short press has happened.
 *
 * @return true
 * @return false
 */
bool short_press_command() {
    uint8_t response[] = {START_PACKAGE, COMMAND_BUTTON_SHORT_PRESS, STOP_PACKAGE};
    HAL_UART_Transmit(huartx, (uint8_t *)&response, sizeof(response), UART_TIMEOUT_MS);
    return true;
}

/**
 * @brief This function is a callback that sends a message
 *  when a long press has happened.
 *
 * @return true
 * @return false
 */
bool long_press_command() {
    uint8_t response[] = {START_PACKAGE, COMMAND_BUTTON_LONG_PRESS, STOP_PACKAGE};
    HAL_UART_Transmit(huartx, (uint8_t *)&response, sizeof(response), UART_TIMEOUT_MS);
    return true;
}

/**
 * @brief Callback for the event of IR floor sensor detected.
 *
 * @return true when everything is ok.
 */
bool ir_floor_command(IrType sensor_type, bool is_detecting) {
    if (sensor_type == FRONT_IR) {
        uint8_t response[] = {START_PACKAGE, COMMAND_DETECT_IR_FRONT, (uint8_t)is_detecting, STOP_PACKAGE};
        HAL_UART_Transmit(huartx, (uint8_t *)&response, sizeof(response), UART_TIMEOUT_MS);
    } else {
        uint8_t response[] = {START_PACKAGE, COMMAND_DETECT_IR_BACK, (uint8_t)is_detecting, STOP_PACKAGE};
        HAL_UART_Transmit(huartx, (uint8_t *)&response, sizeof(response), UART_TIMEOUT_MS);
    }

    return true;
}

void usartComms_init(UART_HandleTypeDef *USARTx) {
    huartx = USARTx;
    HAL_UART_Receive_DMA(huartx, buffer, USART_BUFFER_SIZE);
    EnableIT_IDLE(huartx->Instance);
    pwr_subscribe_off(send_off_command);
    button_subscribe_short_press(short_press_command);
    button_subscribe_long_press(long_press_command);
    irfloor_subscribe(ir_floor_command);
    transmition_sent = false;
    // Led mutex is not taken.
    isLedMutexTaken = osErrorResource;
}

void usartComms_update() {
    uint8_t response[10];
    float   aux, aux2;
    response[0] = START_PACKAGE;
    if (IsActiveFlag_IDLE(huartx->Instance)) {
        // Stop DMA so package can be processed.
        HAL_UART_DMAStop(huartx);
        // If the package has start and stop identifier.
        if (check_package_format()) {
            // Process the data.
            switch (buffer[1]) {
                case COMMAND_ECHO:
                    response[1] = COMMAND_ECHO;
                    response[2] = 0xAA;
                    response[3] = STOP_PACKAGE;
                    HAL_UART_Transmit(huartx, (uint8_t *)&response, 4, UART_TIMEOUT_MS);
                    break;

                case COMMAND_BAT:
                    aux = bat_get_voltage();
                    makeFloat.floatValue = aux;
                    response[1] = COMMAND_BAT;
                    response[2] = makeFloat.bytes[3];
                    response[3] = makeFloat.bytes[2];
                    response[4] = makeFloat.bytes[1];
                    response[5] = makeFloat.bytes[0];
                    response[6] = STOP_PACKAGE;
                    HAL_UART_Transmit(huartx, (uint8_t *)&response, 7, UART_TIMEOUT_MS);
                    break;

                case COMMAND_OFF:
                    pwr_turn_off();
                    break;

                case COMMAND_MOVE:
                    // Get the speed requested.
                    makeFloat.bytes[3] = buffer[2];
                    makeFloat.bytes[2] = buffer[3];
                    makeFloat.bytes[1] = buffer[4];
                    makeFloat.bytes[0] = buffer[5];
                    aux = makeFloat.floatValue;
                    if (fabs(aux) > motor_get_max_speed()) aux = motor_get_max_speed();

                    // Get the radius of the movement..
                    makeFloat.bytes[3] = buffer[6];
                    makeFloat.bytes[2] = buffer[7];
                    makeFloat.bytes[1] = buffer[8];
                    makeFloat.bytes[0] = buffer[9];
                    aux2 = makeFloat.floatValue;

                    targetSpeedL = motor_calculate_speed(MOTOR_L, aux, aux2);
                    targetSpeedR = motor_calculate_speed(MOTOR_R, aux, aux2);
                    break;

                case COMMAND_LED_AQUIRE:
                    if (isLedMutexTaken != osOK) {
                        isLedMutexTaken = osMutexWait(led_mutexHandle, LED_MUTEX_WAIT_TIME_MS);
                    }
                    break;

                case COMMAND_LED_RED_ON:
                    if (isLedMutexTaken == osOK) {
                        HAL_GPIO_WritePin(RED_LED_PORT, RED_LED_PIN, GPIO_PIN_SET);
                    }
                    break;

                case COMMAND_LED_RED_OFF:
                    if (isLedMutexTaken == osOK) {
                        HAL_GPIO_WritePin(RED_LED_PORT, RED_LED_PIN, GPIO_PIN_RESET);
                    }
                    break;

                case COMMAND_LED_GREEN_ON:
                    if (isLedMutexTaken == osOK) {
                        HAL_GPIO_WritePin(GREEN_LED_PORT, GREEN_LED_PIN, GPIO_PIN_SET);
                    }
                    break;

                case COMMAND_LED_GREEN_OFF:
                    if (isLedMutexTaken == osOK) {
                        HAL_GPIO_WritePin(GREEN_LED_PORT, GREEN_LED_PIN, GPIO_PIN_RESET);
                    }
                    break;

                case COMMAND_LED_RELEASE:
                    if (isLedMutexTaken == osOK) {
                        isLedMutexTaken = osMutexRelease(led_mutexHandle);
                    }
                    break;

                case COMMAND_SERVO_MOVE:
                    servo_move(response[2]);
                    break;

                case COMMAND_DETECT_IR_FRONT:
                    buffer[1] = COMMAND_DETECT_IR_FRONT;
                    if (irfloor_is_front_detecting()) {
                        buffer[1] = 1;
                    } else {
                        buffer[1] = 0;
                    }
                    response[2] = STOP_PACKAGE;
                    HAL_UART_Transmit(huartx, (uint8_t *)&response, 3, UART_TIMEOUT_MS);
                    break;

                case COMMAND_DETECT_IR_BACK:
                    buffer[1] = COMMAND_DETECT_IR_BACK;
                    if (irfloor_is_back_detecting()) {
                        buffer[1] = 1;
                    } else {
                        buffer[1] = 0;
                    }
                    response[2] = STOP_PACKAGE;
                    HAL_UART_Transmit(huartx, (uint8_t *)&response, 3, UART_TIMEOUT_MS);
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
                    HAL_UART_Transmit(huartx, (uint8_t *)&response, 3, UART_TIMEOUT_MS);
                    break;
            }
        }
        ClearFlag_IDLE(huartx->Instance);
        MX_USART1_UART_Init();
        HAL_UART_Receive_DMA(huartx, buffer, USART_BUFFER_SIZE);
    }
}
