/**
 * @file PowerManagement.hpp
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Power management class for Noah robot
 * @version 0.1
 * @date 01-07-2020
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

// Standard libraries
// CubeMx libraries
#include <NoahUtils.h>
#include "cmsis_os.h"
#include "main.h"

// Noah libraries

using noah::noah_utils::Pin;

namespace noah {
namespace power_management {

class PowerManagement {
public:
	/**
	 * @brief Default constructor
	 */
	explicit PowerManagement(const Pin &led_1, const Pin &led_2,
			const Pin &en_power_3v3, const Pin &smps_pwr,
			const Pin &enable_pressed, const Pin &charger_connected,
			const Pin &standby_h_bridge);

	/**
	 * @brief Default destructor
	 */
	~PowerManagement() = default;

	/**
	 * @brief executes the actions of this class
	 */
	int run();

private:
	/**
	 * @brief checks if Power ON button was pressed. It uses debouncing.
	 * @return TRUE if button pressed, FALSE otherwise.
	 */
	bool isButtonPressed();

	/**
	 * @brief Checks if it is charging.
	 * @return TRUE if charging, FALSE otherwise.
	 */
	bool isChargerConnected();

	/**
	 * @brief based on the inputs of the system it estimates the Power Management status.
	 * @return current state of the board.
	 */
	void updateState();

	/// @brief Pins related to the fucntionality of power management
	Pin led_1_;
	Pin led_2_;
	Pin en_power_3v3_;
	Pin smps_pwr_;
	Pin enable_pressed_;
	Pin charger_connected_;
	Pin standby_h_bridge_;

	/// @brief Flags for each possible imput of the FSM.
	bool is_robot_on_;
	bool is_robot_charging_;

};

}
}
