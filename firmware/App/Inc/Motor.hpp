/**
 * @file PID
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Library for PWM motor control with an H bridge.
 * @version 0.1
 * @date 11-07-2020
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

// Standard libraries
#include <stdlib.h>
// CubeMx libraries
#include <NoahUtils.h>
#include "main.h"
#include "tim.h"

// Noah libraries

using noah::noah_utils::Pin;
using noah::noah_utils::MotorChannel;

namespace noah {

class Motor {
 public:
  /**
   * @brief Default constructor
   */
  explicit Motor(MotorChannel &channel, const Pin &motor_a, const Pin &motor_b,
                 const uint32_t min_value, const uint32_t max_value) :
      motor_timer_ { channel },
      motor_a_ { motor_a },
      motor_b_ { motor_b },
      min_value_ { min_value },
      max_value_ { max_value } {
    // Start the motor and sets it to off.
    HAL_TIM_PWM_Start(motor_timer_.timer, motor_timer_.channel);
    setPwm(0);
  }

  /**
   * @brief Default destructor
   */
  ~Motor() = default;

  /**
   * @brief sets the speed of the motor. If the duty is positive it has to spin with
   *        the positive orientation (anti-clockwise). If duty negative, it will spin
   *        opposite direction.
   *
   * @param duty desired speed of the motor. It must be between min_value and max_value.
   */
  void setDuty(int32_t duty) {
    if (duty > 0) {
      HAL_GPIO_WritePin(motor_a_.port, motor_a_.pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(motor_b_.port, motor_b_.pin, GPIO_PIN_RESET);
      setPwm(static_cast<uint32_t>(duty));
    } else if (duty == 0) {
      HAL_GPIO_WritePin(motor_a_.port, motor_a_.pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(motor_b_.port, motor_b_.pin, GPIO_PIN_RESET);
      setPwm(0);
    } else {
      HAL_GPIO_WritePin(motor_a_.port, motor_a_.pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(motor_b_.port, motor_b_.pin, GPIO_PIN_SET);
      setPwm(static_cast<uint32_t>(abs(duty)));
    }
  }

  /**
   * @brief returns the current duty of the motor.
   */
  uint32_t getDuty() const {
    return current_duty;
  }

 private:
  /**
   * @brief Sets the duty for the motor that is being used.
   * @param duty value between 0 and max_value.
   */
  void setPwm(uint32_t value) {
    uint32_t duty_value;
    if (value > max_value_) {
      duty_value = max_value_;
    }
    else if (value < min_value_) {
      duty_value = 0;
    }
    else {
      duty_value = value;
    }
    current_duty = duty_value;
    switch (motor_timer_.channel) {
      case TIM_CHANNEL_1:
        motor_timer_.timer->Instance->CCR1 = duty_value;
        break;
      case TIM_CHANNEL_2:
        motor_timer_.timer->Instance->CCR2 = duty_value;
        break;
      case TIM_CHANNEL_3:
        motor_timer_.timer->Instance->CCR3 = duty_value;
        break;
      case TIM_CHANNEL_4:
        motor_timer_.timer->Instance->CCR4 = duty_value;
        break;
      default:
        break;
    }
  }

  /// @brief Timer handler that access to the PWM.
  MotorChannel motor_timer_;

  /// @brief Pins for the direction of the H bridge.
  Pin motor_a_;
  Pin motor_b_;

  /// @brief Limits for the possible duty values.
  const uint32_t min_value_;
  const uint32_t max_value_;

  /// @brief Current duty of the motor.
  int32_t current_duty;

};

}
