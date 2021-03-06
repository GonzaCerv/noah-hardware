/**
 * @file PID
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Library for managing the PID of each motor.
 * @version 0.1
 * @date 01-07-2020
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

// Standard libraries
// CubeMx libraries
#include <Utils/NoahUtils.hpp>
#include "arm_math.h"
#include "cmsis_os.h"
#include "main.h"
#include "tim.h"

// Noah libraries
#include "Config.h"
#include "Utils/Encoder.hpp"
#include "Utils/Motor.hpp"

using noah::utils::Motor;
using noah::utils::Pin;

namespace noah {
namespace tasks {

class PID {
 public:
  /**
   * @brief Default constructor
   *
   * @param motor injected object to drive the motor in the PID.
   * @param max_integration_sum maximum value that is allowed to reach in the integration parameter.
   * @param kp kp value of the PID.
   * @param ki ki value of the PID.
   * @param kd kd value of the PID.
   */
  explicit PID(Motor &motor, const float max_integration_sum = MAX_INTEGRATION_SUM, const float kp = 0.0f,
               const float ki = 0.0f, const float kd = 0.0f);

  /**
   * @brief Default destructor
   */
  ~PID() = default;

  /// @brief Updates the current status of the PID.
  void updatePID(float target_speed, float current_speed);

 private:

  /// @brief Object that manages the motor.
  Motor motor_;

  float current_speed_;

  /// @brief Maximum value that the PID can reach before blocking.
  const float max_integration_sum_;

  /// @brief PID constants.
  const float kp_;
  const float ki_;
  const float kd_;

  /// @brief CMSIS object that calculates the output of the PID.
  arm_pid_instance_f32 pid_;

  /// @brief Differente between the target and the current speed.
  float current_error_;

};

}
}
