/**
 * @file IMU.hpp
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Library for collecting information from IMU.
 * @version 0.1
 * @date 06-03-2021
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

// Standard libraries
#include <memory>

// CubeMx libraries
#include "arm_math.h"
#include "cmsis_os.h"
#include "main.h"
#include "tim.h"

// Noah libraries
#include <Config.h>
#include <External/SparkFun_MPU_9250_DMP/src/SparkFunMPU9250-DMP.h>
#include <Utils/NoahInfo.h>
#include <Utils/NoahUtils.hpp>

namespace noah {
namespace tasks {

class IMU {
 public:
  /**
   * @brief Default constructor
   *
   * @param noah_info_handler Handler for storing information.
   */
  explicit IMU(NoahInfoHandler *noah_info_handler);

  /**
   * @brief Default destructor
   */
  ~IMU() = default;

  /// @brief Updates the current status of the PID.
  int run();

 private:
  /// @brief reference to the info handler.
  NoahInfoHandler *noah_info_handler;

  /// @brief represents the IMU of the robot.
  MPU9250_DMP imu;

};

}
}
