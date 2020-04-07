/**
 * @file PowerManagement.h
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief This library manages how Noah manages the energy
 * @version 0.1
 * @date 2019-12-16
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef POWER_MANAGEMENT_H
#define POWER_MANAGEMENT_H

// Standard libraries
#include "stdbool.h"

// CubeMX libraries
#include "FreeRTOS.h"
#include "cmsis_os.h"

// Noone libraries

/**
 * @brief Enumerates the possible states of the robot.
 *
 */
typedef enum {
    OFF,
    ENTRYPOINT,
    CHARGING_USB,
    CHARGING_HIGH_PWR,
    DISCHARGING,
    CHARGED,
} PwrStates;

/// @brief function prototype that are used as a callback to turn off peripherals.
typedef bool (*offFunPtr)(void);

/**
 * @brief Initialize the object based on the configuration of Config.h
 *
 */
void pwr_init();

/**
 * @brief Checks the different states and determines the current situation of the robot.
 *
 */
bool pwr_update();

/**
 * @brief Get the Current State of the power management
 *
 * @return PwrStates curent state of the system.
 */
PwrStates pwr_get_current_state();

/**
 * @brief Subscribe callback functions that are going to be called when device is off.
 *
 * @param function
 */
void pwr_subscribe_off(offFunPtr function);

/**
 * @brief Turns off the device.
 *
 */
void pwr_turn_off();

/// @brief store if Noah is on or off.
bool isOn;

/// @brief Stores the state of the system.
PwrStates pwr_currentState;

#endif  // !POWER_MANAGEMENT_H