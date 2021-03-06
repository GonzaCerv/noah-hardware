/**
 * @file PowerManagement.h
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Stores information about the whole system. This library is thread safe so it can be called
 *        from different threads.
 * @version 0.1
 * @date 19-07-2020
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef __noah_info_H
#define __noah_info_H

#ifdef __cplusplus
extern "C" {
#endif

// Standard libraries
#include "stdbool.h"

// CubeMx libraries
#include "cmsis_os.h"

// Noah libraries
#include "NoahUtils.hpp"

/**
 * @brief This struct holds the information of the whole system.
 *        It must be used with the methods of this library or it
 *        could lead to data leakage when using in multiple threads.
 */
typedef struct {
  int16_t ticks_l;
  int16_t ticks_r;
  float target_speed_l;
  float target_speed_r;
  Vector3 imu_acceleration;
  Vector3 imu_gyroscope;
  osMutexId *mutexHandle;
} NoahInfoHandler;

/**
 * @brief Initializes the Handler
 * @param mutex mutex that is going to represent the resources.
 * @param info_handler handler that is going to contain the information.
 */
void noahInfo_init(osMutexId *mutex, NoahInfoHandler *info_handler);

bool noahInfo_isInitialized(NoahInfoHandler *info_handler);

/**
 * @brief Sets the ticks to the left encoder
 * @param info_handler struct that holds the information.
 * @param ticks ticks to save.
 */
void noahInfo_set_ticks_l(NoahInfoHandler *info_handler, int16_t ticks);

/**
 * @brief Gets the ticks to the left encoder
 * @param info_handler struct that holds the information.
 * @return current left ticks of the encoder..
 */
int16_t noahInfo_get_ticks_l(NoahInfoHandler *info_handler);

/**
 * @brief Sets the ticks to the right encoder
 * @param info_handler struct that holds the information.
 * @param ticks ticks to save.
 */
void noahInfo_set_ticks_r(NoahInfoHandler *info_handler, int16_t ticks);

/**
 * @brief Gets the ticks of the right encoder
 * @param info_handler struct that holds the information.
 * @return current left ticks of the encoder..
 */
int16_t noahInfo_get_ticks_r(NoahInfoHandler *info_handler);

/**
 * @brief Sets the speed for the left motor
 * @param info_handler struct that holds the information.
 * @param speed speed to save.
 */
void noahInfo_set_target_speed_l(NoahInfoHandler *info_handler, float speed);

/**
 * @brief Gets the speed of the left encoder
 * @param info_handler struct that holds the information.
 * @return current left target speed of the motor.
 */
float noahInfo_get_target_speed_l(NoahInfoHandler *info_handler);

/**
 * @brief Sets the speed for the right motor
 * @param info_handler struct that holds the information.
 * @param speed speed to save.
 */
void noahInfo_set_target_speed_r(NoahInfoHandler *info_handler, float speed);

/**
 * @brief Gets the speed of the right encoder
 * @param info_handler struct that holds the information.
 * @return current left target speed of the motor.
 */
float noahInfo_get_target_speed_r(NoahInfoHandler *info_handler);

/**
 * @brief Sets the acceleration of the imu.
 * @param info_handler struct that holds the information.
 * @param acceleration struct that holds the acceleration.
 */
void noahInfo_set_imu_acceleration(NoahInfoHandler *info_handler, Vector3 acceleration);

/**
 * @brief Gets the acceleration of the right encoder
 * @param info_handler struct that holds the information.
 * @return current acceleration of the IMU.
 */
Vector3 noahInfo_get_imu_acceleration(NoahInfoHandler *info_handler);

#ifdef __cplusplus
}
#endif

#endif
