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
#ifndef USART_COMMS_H
#define USART_COMMS_H

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

#endif  // !USART_COMMS_H
