/**
 * @file PowerManagement.c
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief This library manages how Noah administrates the energy
 * @version 0.1
 * @date 2019-12-16
 *
 * @copyright Copyright (c) 2019
 *
 */
// Standard libraries
#include "stdint.h"

// CubeMX libraries
#include "gpio.h"
#include "usbd_cdc_if.h"

// Noone libraries
#include "Battery.h"
#include "Button.h"
#include "Config.h"
#include "PowerManagement.h"

#define USB_MODE GPIO_PIN_RESET
#define HIGHPOWER_MODE GPIO_PIN_SET

const uint32_t led_mutex_wait_time = 100;

typedef enum { PRECHARGE, FAST_CHARGE, CHARGE_DONE, CHARGE_SUSPENDED } BQ24070_status;

/// @brief Current state of the charger.
BQ24070_status bq24070Status;

/// @brief Stores if the button was pressed or not.
bool buttonPressed;

/// @brief Count of ticks that the button have been pressed.
uint32_t buttonCurrentTickCount;

/// @brief When this bool sets to true, a change in the configurations has changed.
bool newEvent;

/// @brief Stores the connection status of the PC.
bool isConnectedToPc;

/// @brief Array that stores all the functions that are going to be called when Noah goes off.
offFunPtr functionOffArray[10];

/// @brief Counts the amount of functions called when Noah goes off.
uint8_t functionOffCount = 0;

/// @brief Status of the battery.
extern BatteryState batState;

/// @brief this variable allow us to check if the USB device is plugged or not.
extern USBD_HandleTypeDef hUsbDeviceFS;

void BQ24070_set_mode(GPIO_PinState state) { HAL_GPIO_WritePin(BQ24070_MODE_PORT, BQ24070_MODE_PIN, state); }

/**
 * @brief Get the status of the battery charger.
 *
 * @return BQ24070_status Status of the charger.
 */
BQ24070_status BQ24070_get_mode() {
    if ((HAL_GPIO_ReadPin(BQ24070_STAT1_PORT, BQ24070_STAT1_PIN) == GPIO_PIN_RESET) &&
        (HAL_GPIO_ReadPin(BQ24070_STAT2_PORT, BQ24070_STAT2_PIN) == GPIO_PIN_RESET)) {
        return PRECHARGE;
    } else if ((HAL_GPIO_ReadPin(BQ24070_STAT1_PORT, BQ24070_STAT1_PIN) == GPIO_PIN_RESET) &&
               (HAL_GPIO_ReadPin(BQ24070_STAT2_PORT, BQ24070_STAT2_PIN) == GPIO_PIN_SET)) {
        return FAST_CHARGE;
    } else if ((HAL_GPIO_ReadPin(BQ24070_STAT1_PORT, BQ24070_STAT1_PIN) == GPIO_PIN_SET) &&
               (HAL_GPIO_ReadPin(BQ24070_STAT2_PORT, BQ24070_STAT2_PIN) == GPIO_PIN_RESET)) {
        return CHARGE_DONE;
    } else {
        return CHARGE_SUSPENDED;
    }
}

/**
 * @brief This callback is called from button library when
 * a long pressed event has ocurred.
 *
 * @return true
 * @return false
 */
bool button_pressed_callback() {
    buttonPressed = true;
    newEvent = true;
    return true;
}

/**
 * @brief Checks if the USB is plugged or not.
 *
 */
void check_pc_connection() {
    // Check if it is connected or not.
    if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED) {
        isConnectedToPc = true;
        // If no new event was alerted.
        if (!newEvent) newEvent = true;
    } else {
        isConnectedToPc = false;
    }
}

/**
 * @brief Turns off the device.
 *
 */
void turn_off() {
    HAL_GPIO_WritePin(ENABLE_6V_PORT, ENABLE_6V_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RED_LED_PORT,RED_LED_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GREEN_LED_PORT,GREEN_LED_PIN,GPIO_PIN_RESET);
    uint8_t count = 0;
    // Sequence to turn off all the peripherals.
    for (count = 0; count < functionOffCount; ++count) {
        (*functionOffArray[count])();
    }
    // Turn off last power supplies.
    HAL_GPIO_WritePin(ENABLE_5V_PORT, ENABLE_5V_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ENABLE_3V_PORT, ENABLE_3V_PIN, GPIO_PIN_RESET);
}

/**
 * @brief Initializes the power management library.
 *
 */
void pwr_init() {
    newEvent = false;
    batState = OPERATIVE;
    bq24070Status = CHARGE_SUSPENDED;
    buttonPressed = false;
    isConnectedToPc = false;
    isOn = false;
    button_subscribe_long_press(&button_pressed_callback);
    pwr_currentState = ENTRYPOINT;
}

/**
 * @brief Updates the current status of the system.
 *
 */
bool pwr_update() {
    // check if there is a change in the operation of BQ24070.
    if (BQ24070_get_mode() != bq24070Status) {
        newEvent = true;
        bq24070Status = BQ24070_get_mode();
    }
    // Check if Noah is connected via USB to a PC.
    check_pc_connection();

    // Check battery status.
    if (batState != bat_get_state()){
        batState = bat_get_state();
        newEvent = true;
    }

    if (newEvent) {
        // Update the status of the system  (Read Power management state machine for more info).
        switch (pwr_currentState) {
            case ENTRYPOINT:
                if (isConnectedToPc) {
                    HAL_GPIO_WritePin(ENABLE_3V_PORT, ENABLE_3V_PIN, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(ENABLE_5V_PORT, ENABLE_5V_PIN, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(ENABLE_6V_PORT, ENABLE_6V_PIN, GPIO_PIN_RESET);
                    BQ24070_set_mode(USB_MODE);
                    pwr_currentState = CHARGING_USB;
                } else if (buttonPressed && (batState == OPERATIVE)) {
                    HAL_GPIO_WritePin(ENABLE_3V_PORT, ENABLE_3V_PIN, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(ENABLE_5V_PORT, ENABLE_5V_PIN, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(ENABLE_6V_PORT, ENABLE_6V_PIN, GPIO_PIN_SET);
                    isOn = true;
                    pwr_currentState = DISCHARGING;
                } else if (!isConnectedToPc && (bq24070Status != CHARGE_SUSPENDED)) {
                    HAL_GPIO_WritePin(ENABLE_3V_PORT, ENABLE_3V_PIN, GPIO_PIN_SET);
                    BQ24070_set_mode(HIGHPOWER_MODE);
                    pwr_currentState = CHARGING_HIGH_PWR;
                } else if (batState == NO_OPERATIVE) {
                    pwr_currentState = OFF;
                    turn_off();
                }
                break;

            case CHARGING_USB:
                if (!isConnectedToPc && !isOn) {
                    pwr_currentState = OFF;
                    turn_off();
                } else if (buttonPressed && (batState == OPERATIVE) && !isOn) {
                    HAL_GPIO_WritePin(ENABLE_3V_PORT, ENABLE_3V_PIN, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(ENABLE_5V_PORT, ENABLE_5V_PIN, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(ENABLE_6V_PORT, ENABLE_6V_PIN, GPIO_PIN_SET);
                    isOn = true;
                    pwr_currentState = DISCHARGING;
                } else if (!isConnectedToPc && (batState == OPERATIVE) && !isOn) {
                    pwr_currentState = DISCHARGING;
                } else if (batState == NO_OPERATIVE) {
                    pwr_currentState = OFF;
                    turn_off();
                } else if ((bq24070Status == CHARGE_DONE) && (batState == OPERATIVE) && isOn) {
                    pwr_currentState = CHARGED;
                }
                break;

            case CHARGING_HIGH_PWR:
                if (buttonPressed && (batState == OPERATIVE) && !isOn) {
                    HAL_GPIO_WritePin(ENABLE_3V_PORT, ENABLE_3V_PIN, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(ENABLE_5V_PORT, ENABLE_5V_PIN, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(ENABLE_6V_PORT, ENABLE_6V_PIN, GPIO_PIN_SET);
                    isOn = true;
                    pwr_currentState = DISCHARGING;
                } else if (buttonPressed && (batState == OPERATIVE) && isOn) {
                    pwr_currentState = OFF;
                    turn_off();
                } else if ((bq24070Status == CHARGE_SUSPENDED) && (batState == OPERATIVE) && isOn) {
                    pwr_currentState = DISCHARGING;
                } else if (batState == NO_OPERATIVE) {
                    pwr_currentState = OFF;
                    turn_off();
                } else if ((bq24070Status == CHARGE_DONE) && (batState == OPERATIVE) && isOn) {
                    pwr_currentState = CHARGED;
                }
                break;

            case CHARGED:
                if (!isConnectedToPc && (bq24070Status != CHARGE_DONE) && (batState == OPERATIVE) && isOn) {
                    pwr_currentState = DISCHARGING;
                } else if (isConnectedToPc) {
                    pwr_currentState = CHARGING_USB;
                } else if (!isConnectedToPc && (bq24070Status != CHARGE_SUSPENDED)) {
                    pwr_currentState = CHARGING_HIGH_PWR;
                } else if (buttonPressed && isOn) {
                    pwr_currentState = OFF;
                    turn_off();
                } else if ((batState == NO_OPERATIVE)) {
                    pwr_currentState = OFF;
                    turn_off();
                }
                break;

            case DISCHARGING:
                if (buttonPressed && (batState == OPERATIVE) && !isOn) {
                    pwr_currentState = CHARGING_USB;
                    BQ24070_set_mode(USB_MODE);
                } else if(!isConnectedToPc && (bq24070Status != CHARGE_SUSPENDED)){
                    pwr_currentState = CHARGING_HIGH_PWR;
                    BQ24070_set_mode(HIGHPOWER_MODE);
                } else if (buttonPressed && isOn) {
                    pwr_currentState = OFF;
                    turn_off();
                } else if (batState == NO_OPERATIVE) {
                    pwr_currentState = OFF;
                    turn_off();
                }
                break;

            default:
                break;
        }
        newEvent = false;
        return true;
    }
    return false;
}

/**
 * @brief Returns the current state of the system.
 *
 * @return PwrStates state of the system.
 */
PwrStates pwr_get_current_state() { return pwr_currentState; }

/**
 * @brief Subscribe callback functions that are going to be called when device is off.
 *
 * @param function
 */
void pwr_subscribe_off(offFunPtr function) {
    functionOffArray[functionOffCount] = function;
    ++functionOffCount;
}

/**
 * @brief Turns off the device.
 *
 */
void pwr_turn_off() { turn_off(); }