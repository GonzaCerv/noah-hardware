/**
 * @file IMUNode.cpp
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Entrypoint for the node in charge managing IMU gathering.
 * @version 0.1
 * @date 07-02-2021
 *
 * @copyright Copyright (c) 2020
 *
 */

// Standard libraries
#include <cstdlib>

// Noah libraries
#include "Config.h"
#include "NodeEntrypoint.h"
#include "Tasks/IMU/IMU.hpp"
#include "Utils/NoahInfo.h"

using noah::tasks::IMU;

// External variable definitions

/**
 * @brief Initializes the power management node
 * @return
 */
int IMUNode(NoahInfoHandler *noah_info_handler) {

  IMU imu_node(noah_info_handler);

  imu_node.run();
  return EXIT_SUCCESS;
}

