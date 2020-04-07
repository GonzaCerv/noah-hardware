/**
 * @file Button.c
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief This function manages the button behaviour
 * @version 0.1
 * @date 2019-12-20
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "Button.h"
#include "Config.h"
#include "stdint.h"

bool     buttonPressed = false;
bool     functionsCalled = false;
funPtr   longPressArray[10];
uint8_t  longPressCount = 0;
funPtr   shortPressArray[10];
uint8_t  shortPressCount = 0;
uint32_t tickCount = 0;

/**
 * @brief Call all the callbacks for long press of the button.
 *
 * @return true all the function were called successfully.
 * @return false some functions had errors.
 */
bool call_long_press_callbacks() {
    uint8_t count = 0;
    bool    result = true;
    for (count = 0; count < longPressCount; ++count) {
        bool success = (*longPressArray[count])();
        if (!success) result = false;
    }
    return result;
}

bool call_short_press_callbacks() {
    uint8_t count = 0;
    bool    result = true;
    for (count = 0; count < longPressCount; ++count) {
        bool success = (*longPressArray[count])();
        if (!success) result = false;
    }
    return result;
}

void button_subscribe_short_press(funPtr function) {
    shortPressArray[shortPressCount] = function;
    ++shortPressCount;
}

void button_subscribe_long_press(funPtr function) {
    longPressArray[longPressCount] = function;
    ++longPressCount;
}

void button_update() {
    // This case is when the button is pressed the first time.
    if ((HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_SET) && (buttonPressed == false)) {
        tickCount = HAL_GetTick();
        buttonPressed = true;
    }
    // This is while the button is being pressed.
    else if ((HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_SET) && (buttonPressed == true)) {
        if (((HAL_GetTick() - tickCount) > BUTTON_LONG_PRESS_TICK) && (functionsCalled == false)) {
            call_long_press_callbacks();
            functionsCalled = true;
        }
    }
    // This is when the button is released.
    else if ((HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_RESET) && (buttonPressed == true)) {
        // This is in the case that the button is released between the long and the short press.
        if (((HAL_GetTick() - tickCount) < BUTTON_LONG_PRESS_TICK) &&
            ((HAL_GetTick() - tickCount) > BUTTON_SHORT_PRESS_TICK) && (functionsCalled == false)) {
            call_short_press_callbacks();
        }
        functionsCalled = false;
        buttonPressed = false;
        tickCount = 0;
    }
}