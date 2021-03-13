/**
 * @file PID
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief This library retrieves the current count of an encoder.
 *
 * @version 0.1
 * @date 11-07-2020
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

// Standard libraries
#include <Utils/NoahUtils.hpp>
#include <algorithm>
#include <cmath>
#include <limits>

// CubeMx libraries
#include "cmsis_os.h"
#include "main.h"
#include "tim.h"

// Noah libraries

namespace noah {
namespace utils {

class Encoder {
 public:
  /**
   * @brief Default constructor
   * @param message_queue_id memory queue from which read the current tick count.
   * @param timer_encoder_handler handler of the timer that counts the ticks from the encoder.
   * @param interrupt_rate_ms value used to calculate speed. It must match the time taken to timer_interrupt_handler to generate an interrupt.
   * @param steps_per_turn number of ticks that the encoder generates in every turn.
   * @param wheel_radius radius of the wheel used to calculate the current speed.
   */
  explicit Encoder(osMessageQId &message_queue_id, TIM_HandleTypeDef &timer_encoder_handler,
                   const float interrupt_rate_ms, const float steps_per_turn, const float wheel_radius) :
      message_queue_id_ { message_queue_id },
      timer_encoder_handler_ { timer_encoder_handler },
      interrupt_rate_ms_ { interrupt_rate_ms },
      steps_per_turn_ { steps_per_turn },
      wheel_radius_ { wheel_radius } {
    HAL_TIM_Encoder_Start_IT(&timer_encoder_handler_, TIM_CHANNEL_ALL);
  }
  ;

  /**
   * @brief Returns the current tick count of the encoder. This method blocks until
   * 		  a new value gets published in the memory queue.
   * @return current tick count.
   */
  int16_t getTicks() {
    // Try to get a new message
    osEvent evt = osMessageGet(message_queue_id_, osWaitForever);
    // If the message is OK, return the tick.
    if (evt.status == osEventMessage) {
      return evt.value.v;
    }
    // If any error occurs, return 0.
    return 0;
  }
  ;

  /**
   * @brief Reads a new tick count from the timers and, based on the last tick count,
   * 		  calculates the current speed of the motor.
   * @return current tick count.
   */
  float getCurrentSpeed(int16_t previous_ticks, int16_t current_ticks) {

    // Calculate the current tick difference.
    auto current_diff = encoder_calculate_diff(previous_ticks, current_ticks);

    // Calculate the speed.
    auto result = ((((float) current_diff / (interrupt_rate_ms_ / 1000.0f)) / steps_per_turn_) * 2 * (float) M_PI
        * wheel_radius_);
    result = applyResolution(result, SPEED_RESOLUTION);
    return result;
  }
  ;

  /**
   * @brief Default destructor
   */
  ~Encoder() = default;

 private:
  /**
   * @brief Based on the values of current_ticks and previous ticks, checks if the count
   * 		  has overflowed or underflowed.
   * @return TRUE if has wrapped, FALSE otherwise.
   */
  bool has_wrapped(int16_t previous, int16_t current) {
    if ((std::min(previous, current) < (negative_wrap_value_))
        && (std::max(previous, current) > positive_wrap_value_)) {
      return true;
    }
    return false;
  }

  /**
   * @brief calculates the difference between the current and the previous ticks adquired. It takes into
   *        account if the count is incrementing, drecrementing, overflowed or underflowed.
   * @return
   */
  int16_t encoder_calculate_diff(int16_t previous_ticks, int16_t current_ticks) {
    int16_t result;
    if (has_wrapped(previous_ticks, current_ticks)) {
      int16_t ticks_before_wrap = 0;
      int16_t ticks_after_wrap = 0;
      if (previous_ticks < 0) {
        ticks_before_wrap = previous_ticks + std::numeric_limits < int16_t > ::max();
      } else {
        ticks_before_wrap = (-std::numeric_limits < int16_t > ::min()) - previous_ticks;
      }
      if (current_ticks < 0) {
        ticks_after_wrap = current_ticks + std::numeric_limits < int16_t > ::max();
      } else {
        ticks_after_wrap = (-std::numeric_limits < int16_t > ::min()) - previous_ticks;
      }

      result = ticks_after_wrap + ticks_before_wrap;

      // Check if the speed is negative.
      if (current_ticks > 0) {
        result = -result;
      }

    } else {
      result = std::max(previous_ticks, current_ticks) - std::min(previous_ticks, current_ticks);
      if (current_ticks < previous_ticks) {
        result = -result;
      }
    }
    return result;
  }
  ;

  /// @brief Reference to the memory queue.
  osMessageQId message_queue_id_;

  /// @brief Handler for each of the timers.
  TIM_HandleTypeDef timer_encoder_handler_;

  /// @brief Time that must wait before next check of ticks.
  float interrupt_rate_ms_;

  /// @brief Number of steps in the encoder per turn in the axis of the wheel.
  const float steps_per_turn_;

  /// @brief Radius of the wheel.
  const float wheel_radius_;

  //// @brief If previous_ticks_ is below this value and then is above, it is considered as underflow.
  ///  corresponds to 1/4 of the total range of a int16_t.
  const int16_t negative_wrap_value_ = -13107;

  //// @brief If previous_ticks_ is above this value and then is below, it is considered as overflow.
  ///  corresponds to 3/4 of the total range of a int16_t.
  const int16_t positive_wrap_value_ = 13106;

};

}
}
