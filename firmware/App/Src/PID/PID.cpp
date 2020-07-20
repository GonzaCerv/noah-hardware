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
#include "PID/PID.hpp"

extern float target_speed_;
extern float current_speed_;

namespace noah {
namespace pid {

PID::PID(Motor &motor,
         const float max_integration_sum, const float kp, const float ki, const float kd) :
    motor_ { motor },
    max_integration_sum_ { max_integration_sum },
    kp_ { kp },
    ki_ { ki },
    kd_ { kd } {
  // Set all variables to the reset state.
  current_speed_ = 0.0f;
  current_error_ = 0.0f;

  // Initialize the pid instance
  pid_.Kp = kp_;
  pid_.Ki = ki_;
  pid_.Kd = kd_;
  arm_pid_init_f32(&pid_, 1);
}

void PID::updatePID(float target_speed, float current_speed) {

  // Calculate the PID.
  current_error_ = target_speed - current_speed;
  int32_t duty_ = static_cast<int32_t>(arm_pid_f32(&pid_, current_error_));

  // Change the speed of the motor.
  motor_.setDuty(duty_);
}

}

}
