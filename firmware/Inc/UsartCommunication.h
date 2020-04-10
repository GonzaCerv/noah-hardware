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

#pragma once

// Standard libraries
#include "stdbool.h"

// CubeMx libraries
#include "usart.h"

// Noah libraries
#include "NoahUtils.h"

/**
 * @brief Initializes the service for USART.
 *
 */
void usartComms_init();

/**
 * @brief Checks if there is new data and process it.
 *
 */
void usartComms_update();

/**
 * @brief Sends the command to turn off the RPI.
 *
 */
void usartComms_publish_off();

/**
 * @brief Returns the last PWM duty for motor L.
 *
 */
float usartComms_get_target_speed_l();

/**
 * @brief Returns the last PWM duty for motor R.
 *
 */
float usartComms_get_target_speed_r();

/**
 * @brief returns the flag that indicates if the robot should
 *        be turned off
 * @return true when robot must be turned off
 */
bool usartComms_is_turn_off_requested();