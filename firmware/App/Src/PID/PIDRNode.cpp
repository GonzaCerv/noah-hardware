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
#include "Encoder.hpp"
#include "Motor.hpp"
#include "NoahInfo.h"
#include "NodeEntrypoint.h"
#include "PID/PID.hpp"

using noah::Encoder;
using noah::Motor;
using noah::pid::PID;
using noah::noah_utils::MotorChannel;
using noah::noah_utils::Pin;

// External variable definitions
extern osMessageQId msg_q_encoder_rHandle;

/**
 * @brief Initializes the power management node
 * @return
 */
int PIDRNode(NoahInfoHandler *noah_info_handler) {
  // Stores the tick count of the encoder.
  int16_t current_ticks = 0;
  int16_t previous_ticks = 0;
  float current_speed = 0;

  // Define all the pins
  Pin motor_r_a { MOT_R1_GPIO_Port, MOT_R1_Pin };
  Pin motor_r_b { MOT_R2_GPIO_Port, MOT_R2_Pin };

  // Timer definitions
  MotorChannel motor_duty_r { &MOTOR_TIMER, MOTOR_CHANNEL_R };

  // Build the objects to inject
  Motor motor_r(motor_duty_r, motor_r_a, motor_r_b, PWM_MIN_VAL, PWM_MAX_VAL);
  Encoder encoder_r(msg_q_encoder_rHandle, ENCODER_TIMER_R, ENCODER_UPDATE_RATE_MS, STEPS_PER_TURN, WHEEL_RADIUS);
  PID pid_r(motor_r, 1.0f, PID_DEFAULT_KP, PID_DEFAULT_KI, PID_DEFAULT_KD);
  while (1) {
    // Get the current tick count.
    previous_ticks = current_ticks;
    current_ticks = encoder_r.getTicks();
    // Store the current ticks
    noahInfo_set_ticks_r(noah_info_handler, current_ticks);
    // Calculate the current speed.
    current_speed = encoder_r.getCurrentSpeed(previous_ticks, current_ticks);
    // Update PID.
    pid_r.updatePID(noahInfo_get_target_speed_r(noah_info_handler), current_speed);

    osDelay(ENCODER_UPDATE_RATE_MS);
  }
  return EXIT_SUCCESS;
}

