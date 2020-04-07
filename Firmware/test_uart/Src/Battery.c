/**
 * @file Battery.c
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Battery checking library
 * @version 0.1
 * @date 2019-12-21
 *
 * @copyright Copyright (c) 2019
 *
 */

// Standard libraries
#include "stdint.h"

// Noone libraries
#include "Battery.h"

/// @brief Stores the status of the conversion.
AdcConversionStatus status;

/// @brief Status of the battery.
BatteryState batState;

float battery_voltage;
/**
 * @brief Initializes the battery checking system.
 *
 */
void bat_init() {
    status = ADC_STOPPED;
    batState = OPERATIVE;
    battery_voltage = 0;
}

/**
 * @brief Checks the battery status and updates the state of the battery.
 *
 */
void bat_update() {
    uint32_t adcBin;
    switch (status) {
        case ADC_STOPPED:
            HAL_ADC_Start_IT(&hadc1);
            status = ADC_CONVERTING;
            break;

        case ADC_CONVERTING:
            break;

        case ADC_FINISHED:
            // Gets the ADC value and calculates the voltage of the battery.
            adcBin = HAL_ADC_GetValue(&hadc1);
            battery_voltage = (float)adcBin * (3.3 / ADC_RES) * (ADC_RSUP + ADC_RINF) / (ADC_RINF) * ADC_SCALE_OFFSET;
            if (battery_voltage > BAT_OPERATIVE_THRESHOLD)
                batState = OPERATIVE;
            else
                batState = NO_OPERATIVE;
            break;
    }
}

BatteryState bat_get_state() { return batState; }

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) { status = ADC_FINISHED; }

float bat_get_voltage() { return battery_voltage; }