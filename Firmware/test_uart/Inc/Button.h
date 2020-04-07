/**
 * @file Button.h
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief This function manages the button behaviour
 * @version 0.1
 * @date 2019-12-20
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef BUTTON_H
#define BUTTON_H

#include "stdbool.h"

typedef bool (*funPtr)(void);

void button_subscribe_short_press(funPtr function);

void button_subscribe_long_press(funPtr function);

void button_update();

#endif  // ! BUTTON_H