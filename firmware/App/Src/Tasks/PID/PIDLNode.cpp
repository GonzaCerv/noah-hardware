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
#include "Config.h"
#include "NodeEntrypoint.h"
#include "Tasks/PID/PID.hpp"
#include "Utils/Encoder.hpp"
#include "Utils/Motor.hpp"
#include "Utils/NoahInfo.h"

using noah::utils::Encoder;
using noah::utils::Motor;
using noah::tasks::PID;
using noah::utils::MotorChannel;
using noah::utils::Pin;

// External variable definitions
extern osMessageQId msg_q_encoder_lHandle;
extern osMessageQId msg_q_speed_lHandle;

/**
 * @brief Initializes the power management node
 * @return
 */
int PIDLNode(NoahInfoHandler *noah_info_handler) {
  // Stores the tick count of the encoder.
  int16_t current_ticks = 0;
  int16_t previous_ticks = 0;
  float current_speed = 0;

  // Define all the pins
  Pin motor_l_a { MOT_L1_GPIO_Port, MOT_L1_Pin };
  Pin motor_l_b { MOT_L2_GPIO_Port, MOT_L2_Pin };

  // Timer definitions
  MotorChannel motor_duty_l { &MOTOR_TIMER, MOTOR_CHANNEL_L };

  // Build the objects to inject
  Motor motor_l(motor_duty_l, motor_l_a, motor_l_b, PWM_MIN_VAL, PWM_MAX_VAL);
  Encoder encoder_l(msg_q_encoder_lHandle, ENCODER_TIMER_L, ENCODER_UPDATE_RATE_MS, STEPS_PER_TURN, WHEEL_RADIUS);
  // Initialize the timer that generates the interrupt that reads the ticks. It will be initialized only after both timers
  // of the encoders are initialized.
  while ((ENCODER_TIMER_L.State != HAL_TIM_StateTypeDef::HAL_TIM_STATE_READY)
      || (ENCODER_TIMER_R.State != HAL_TIM_StateTypeDef::HAL_TIM_STATE_READY)) {
    osDelay(2);
  }
  HAL_TIM_Base_Start_IT(&ENCODER_TIMER_INT);

  PID pid_l(motor_l, 1.0f, PID_DEFAULT_KP, PID_DEFAULT_KI, PID_DEFAULT_KD);
  while (1) {
    // Get the current tick count.
    previous_ticks = current_ticks;
    current_ticks = encoder_l.getTicks();
    // Store the current ticks
    noahInfo_set_ticks_l(noah_info_handler, current_ticks);
    // Calculate the current speed.
    current_speed = encoder_l.getCurrentSpeed(previous_ticks, current_ticks);
    // Update PID.
    pid_l.updatePID(noahInfo_get_target_speed_l(noah_info_handler), current_speed);

    osDelay(ENCODER_UPDATE_RATE_MS);
  }
  return EXIT_SUCCESS;
}
