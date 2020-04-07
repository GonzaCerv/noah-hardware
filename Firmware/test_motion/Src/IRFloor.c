/**
 * @file IRFloor.c
 * @author Gonzalo Cervetti (you@domain.com)
 * @brief Library for floor detection
 * @version 0.1
 * @date 2019-12-31
 *
 * @copyright Copyright (c) 2019
 *
 */
// CubeMx libraries
#include "gpio.h"

// Noah libraries
#include "Config.h"
#include "IRFloor.h"

/**
 * @brief Stores all the pointers to functions
 *
 */
funPtrIr subscribedFunctions[5];
uint8_t  subscribedCount = 0;

/**
 * @brief Checks if the front sensor detects the floor.
 *
 * @return true if floor is detected.
 * @return false if it is not detected.
 */
bool irfloor_is_front_detecting() {
    if (HAL_GPIO_ReadPin(FLOOR_IRF_PORT, FLOOR_IRF_PIN) == GPIO_PIN_SET)
        return true;
    else
        return false;
}

/**
 * @brief Checks if the back sensor detects the floor.
 *
 * @return true if floor is detected.
 * @return false if it is not detected.
 */
bool irfloor_is_back_detecting() {
    if (HAL_GPIO_ReadPin(FLOOR_IRB_PORT, FLOOR_IRB_PIN) == GPIO_PIN_SET)
        return true;
    else
        return false;
}

/**
 * @brief Checks if the back sensor detects the floor.
 *
 * @return true if floor is detected.
 * @return false if it is not detected.
 */
void irfloor_subscribe(funPtrIr ptr) {
    if (subscribedCount < sizeof(subscribedFunctions)) {
        subscribedFunctions[subscribedCount] = ptr;
    }
}

void irfloor_advertise_Change(IrType ir_type, bool is_pressed) {
    uint32_t count = 0;
    for (count = 0; count < subscribedCount; ++count) {
        subscribedFunctions[count](ir_type, is_pressed);
    }
}