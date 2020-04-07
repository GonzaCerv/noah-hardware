/**
 * @file Battery.h
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Battery checking library
 * @version 0.1
 * @date 2019-12-21
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef BATTERY_H
#define BATTERY_H

// CubeMX libraries
#include "adc.h"

// Noone libraries
#include "Config.h"

typedef enum { NO_OPERATIVE, OPERATIVE } BatteryState;

typedef enum { ADC_STOPPED, ADC_CONVERTING, ADC_FINISHED } AdcConversionStatus;

/**
 * @brief Initializes the battery checking system.
 *
 */
void bat_init();

/**
 * @brief Checks the battery status and updates the state of the battery.
 *
 */
void bat_update();

/**
 * @brief get the status of the battery
 *
 * @return BatteryState OPERATIVE when is ready to use, NO_OPERATIVE when is not the case.
 */
BatteryState bat_get_state();

/**
 * @brief Returns the voltage of the battery
 *
 * @return float value of the battery in volts.
 */
float bat_get_voltage();

#endif  // !BATTERY_H
