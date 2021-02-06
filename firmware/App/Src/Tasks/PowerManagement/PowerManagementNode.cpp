/**
 * @file PowerManagementNode.cpp
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Entrypoint for the node in charge of PowerManagement
 * @version 0.1
 * @date 01-07-2020
 *
 * @copyright Copyright (c) 2020
 *
 */

// Noah libraries
#include <Config.h>
#include "NodeEntrypoint.h"
#include "Tasks/PowerManagement/PowerManagement.hpp"

using noah::tasks::PowerManagement;
/**
 * @brief Initializes the power management node
 * @return
 */
int PowerManagementNode() {

	Pin led_1 { AUX_LED1_GPIO_Port, AUX_LED1_Pin };
	Pin led_2 { AUX_LED2_GPIO_Port, AUX_LED2_Pin };
	Pin en_pwr_3v3 { EN_POWER_3V3_GPIO_Port, EN_POWER_3V3_Pin };
	Pin smps_pwr { EN_SMPS_GPIO_Port, EN_SMPS_Pin };
	Pin enable_pressed { ENABLE_PRESSED_GPIO_Port, ENABLE_PRESSED_Pin };
	Pin charger_connected { CHARGER_CONNECTED_GPIO_Port, CHARGER_CONNECTED_Pin };
	Pin mot_stand_by { MOT_STAND_BY_GPIO_Port, MOT_STAND_BY_Pin };
	PowerManagement power_management(led_1, led_2, en_pwr_3v3, smps_pwr,
			enable_pressed, charger_connected, mot_stand_by);

	return power_management.run();
}

