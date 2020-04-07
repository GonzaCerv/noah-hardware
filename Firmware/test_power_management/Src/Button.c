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

bool     button_pressed = false;
bool     functionsCalled = false;
funPtr   longPressArray[10];
uint8_t  longPressCount = 0;
funPtr   shortPressArray[10];
uint8_t  shortPressCount = 0;
uint32_t start_tick_count = 0;

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
    for (count = 0; count < shortPressCount; ++count) {
        bool success = (*shortPressArray[count])();
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
    if(button_pressed){
        uint32_t current_tick;
        if(HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN)== GPIO_PIN_SET){
            current_tick = HAL_GetTick();
            if((current_tick - start_tick_count) > BUTTON_LONG_PRESS_TICK){
                button_pressed = false;
                call_long_press_callbacks();
            }
        }
        else{
            button_pressed = false;
            current_tick = HAL_GetTick();
            if((current_tick - start_tick_count) >= BUTTON_LONG_PRESS_TICK)
                call_long_press_callbacks();
            if((current_tick - start_tick_count) > BUTTON_SHORT_PRESS_TICK)
                call_short_press_callbacks();
        }
    }
    else{
        if (HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_SET){
            button_pressed = true;
            start_tick_count = HAL_GetTick();
        }
    }
}