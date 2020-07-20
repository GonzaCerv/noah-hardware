/**
 * @file PowerManagement.cpp
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Power management class for Noah robot
 * @version 0.1
 * @date 01-07-2020
 *
 * @copyright Copyright (c) 2020
 *
 */

// Standard libraries
#include "stdlib.h"

// CubeMx libraries
#include "stm32f407xx.h"

// Noah libraries
#include "PowerManagement/PowerManagement.hpp"

namespace noah {
namespace power_management {

PowerManagement::PowerManagement(const Pin &led_1, const Pin &led_2,
		const Pin &en_power_3v3, const Pin &smps_pwr, const Pin &enable_pressed,
		const Pin &charger_connected, const Pin &standby_h_bridge) :
		led_1_ { led_1 }, led_2_ { led_2 }, en_power_3v3_ { en_power_3v3 }, smps_pwr_ {
				smps_pwr }, enable_pressed_ { enable_pressed }, charger_connected_ {
				charger_connected }, standby_h_bridge_{standby_h_bridge} {
	// Force all the pins to off
	HAL_GPIO_WritePin(led_1_.port, led_1_.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(led_2_.port, led_2_.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(smps_pwr_.port, smps_pwr_.pin, GPIO_PIN_RESET);

	// Set the initial state of each variable.
	is_robot_on_ = false;
	is_robot_charging_ = false;
}

int PowerManagement::run() {
	while (1) {
		updateState();
		osDelay(200);
	}
	return EXIT_SUCCESS;
}

bool PowerManagement::isButtonPressed() {
	if (HAL_GPIO_ReadPin(enable_pressed_.port, enable_pressed_.pin)
			== GPIO_PIN_SET) {
		osDelay(30);
		if (HAL_GPIO_ReadPin(enable_pressed_.port, enable_pressed_.pin)
				== GPIO_PIN_SET) {
			return true;
		}
	}
	return false;
}

bool PowerManagement::isChargerConnected() {
	if (HAL_GPIO_ReadPin(charger_connected_.port, charger_connected_.pin)
			== GPIO_PIN_SET) {
		return true;
	}
	return false;
}

void PowerManagement::updateState() {
	if ((isButtonPressed()) && (!is_robot_on_)) {
		is_robot_on_ = true;
		HAL_GPIO_WritePin(en_power_3v3_.port, en_power_3v3_.pin, GPIO_PIN_SET);
		dinamicSetPinToOutput(en_power_3v3_);
		HAL_GPIO_WritePin(led_1_.port, led_1_.pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(smps_pwr_.port, smps_pwr_.pin, GPIO_PIN_SET);
		// Wait until the button is released.
		do {
			osDelay(10);
		} while (HAL_GPIO_ReadPin(enable_pressed_.port, enable_pressed_.pin)
				== GPIO_PIN_SET);
		// Turn on H bridge.
		HAL_GPIO_WritePin(standby_h_bridge_.port, standby_h_bridge_.pin, GPIO_PIN_SET);
	} else if ((isButtonPressed()) && (is_robot_on_)) {
		is_robot_on_ = false;
		HAL_GPIO_WritePin(led_1_.port, led_1_.pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(smps_pwr_.port, smps_pwr_.pin, GPIO_PIN_RESET);
		// Wait until the button is released.
		do {
			osDelay(10);
		} while (HAL_GPIO_ReadPin(enable_pressed_.port, enable_pressed_.pin)
				== GPIO_PIN_SET);
		// Turn off H bridge.
		HAL_GPIO_WritePin(standby_h_bridge_.port, standby_h_bridge_.pin, GPIO_PIN_RESET);
		dinamicSetPinToInput(en_power_3v3_);
	}
	if (isChargerConnected()) {
		is_robot_charging_ = true;
		HAL_GPIO_TogglePin(led_1_.port, led_1_.pin);
	}
	if ((!isChargerConnected()) && (is_robot_charging_)) {
		is_robot_charging_ = false;
		if (is_robot_on_) {
			HAL_GPIO_WritePin(led_1_.port, led_1_.pin, GPIO_PIN_SET);
			return;
		}
		HAL_GPIO_WritePin(led_1_.port, led_1_.pin, GPIO_PIN_RESET);
	}
}

}

}

