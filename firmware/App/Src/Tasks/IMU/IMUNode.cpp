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

// Standard libraries
#include <memory>

// Noah libraries
#include "Config.h"
#include "NodeEntrypoint.h"
#include "Tasks/IMU/IMU.hpp"
#include "Utils/NoahInfo.h"

using noah::tasks::IMU;

/**
 * @brief Initializes IMU node
 * @return
 */
int IMUNode(NoahInfoHandler *noah_info_handler) {
  IMU imu(noah_info_handler);
  auto result = imu.run();
  return result;
}
