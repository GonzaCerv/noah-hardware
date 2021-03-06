/**
 * @file IMU
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Library for managing the IMU sensor.
 * @version 0.1
 * @date 01-07-2020
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

// Standard libraries
// CubeMx libraries
#include "cmsis_os.h"
#include "main.h"
#include "Utils/NoahInfo.h"

// Noah libraries

namespace noah {
namespace tasks {

class IMU {
 public:
  /**
   * @brief Default constructor
   *
   */
  explicit IMU(const float update_rate, NoahInfoHandler *noah_info_handler);

  /**
   * @brief Default destructor
   */
  ~IMU() = default;

  /**
   * @brief executes the actions of this class
   */
  int run();

 private:
  void processIncommingPackage();

  /**
   * @brief Enables the interrupt that checks when the USART port is in idle state
   */
  void enableUsartIdleIT();

  /**
   * @brief Checks if new data is available in the port. To do this, it checks whether
   *        the interrupt of idle line was fired  or not.
   * @return true if new data is available, false otherwise.
   */
  bool isNewDataAvailable();

  /**
   * @brief When the idle line interrupt was fired, the flag must be cleared with this function.
   */
  void clearFlagUsartIdleIT();

  /// @brief Port to communicate with ROS.
  UART_HandleTypeDef ros_port_;

  /// @brief Update rate of the calculation of PID
  const float update_rate_;

  /// @brief Buffer for incomming data
  uint8_t buffer_[UART_BUFFER_SIZE];

  /// @brief Allows to retrieve info about the status of the robot.
  NoahInfoHandler *noah_info_handler_;

};

}
}
