/**
 * @file IRFloor.h
 * @author Gonzalo Cervetti (you@domain.com)
 * @brief Library for floor detection
 * @version 0.1
 * @date 2019-12-31
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef IRFLOOR_H
#define IRFLOOR_H

// Standard libraries
#include "stdbool.h"

typedef enum { FRONT_IR, BACK_IR } IrType;

typedef bool (*funPtrIr)(IrType, bool);

/**
 * @brief Checks if the front sensor detects the floor.
 *
 * @return true if floor is detected.
 * @return false if it is not detected.
 */
bool irfloor_is_front_detecting();

/**
 * @brief Subscribe a message for when a s
 *
 * @return true if floor is detected.
 * @return false if it is not detected.
 */
bool irfloor_is_back_detecting();

/**
 * @brief Sends a message to the subscribed function when a change in sensors happends.
 *
 */
void irfloor_subscribe(funPtrIr ptr);

/**
 * @brief Advertise a change in the callbacks subscribed.
 *
 * @param ir_type Type of sensor that is going to be advertised
 * @param is_pressed true if pressed, false elsewhere.
 */
void irfloor_advertise_Change(IrType ir_type, bool is_pressed);

#endif  // !IRFLOOR_H
