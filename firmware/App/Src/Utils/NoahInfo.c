/**
 * @file PowerManagement.c
 * @author Gonzalo Cervetti (cervetti.g@gmail.com)
 * @brief Stores information about the whole system. This library is thread safe so it can be called
 *        from different threads.
 * @version 0.1
 * @date 19-07-2020
 *
 * @copyright Copyright (c) 2020
 *
 */

// Standard libraries
#include "stdbool.h"

// Noah libraries
#include "Utils/NoahInfo.h"
#include "NoahUtils.hpp"

void noahInfo_init(osMutexId *mutex, NoahInfoHandler *info_handler) {
  info_handler->mutexHandle = mutex;
  info_handler->ticks_l = 0;
  info_handler->ticks_r = 0;
  info_handler->target_speed_l = 0.0f;
  info_handler->target_speed_r = 0.0f;
}

bool noahInfo_isInitialized(NoahInfoHandler *info_handler) {
  if (info_handler->mutexHandle) {
    return true;
  }
  return false;
}

void noahInfo_set_ticks_l(NoahInfoHandler *info_handler, int16_t ticks) {
  osMutexWait(*(info_handler->mutexHandle), osWaitForever);
  info_handler->ticks_l = ticks;
  osMutexRelease(*(info_handler->mutexHandle));
}

int16_t noahInfo_get_ticks_l(NoahInfoHandler *info_handler) {
  int16_t ticks;
  osMutexWait(*(info_handler->mutexHandle), osWaitForever);
  ticks = info_handler->ticks_l;
  osMutexRelease(*(info_handler->mutexHandle));
  return ticks;
}

void noahInfo_set_ticks_r(NoahInfoHandler *info_handler, int16_t ticks) {
  osMutexWait(*(info_handler->mutexHandle), osWaitForever);
  info_handler->ticks_r = ticks;
  osMutexRelease(*(info_handler->mutexHandle));
}

int16_t noahInfo_get_ticks_r(NoahInfoHandler *info_handler) {
  int16_t ticks;
  osMutexWait(*(info_handler->mutexHandle), osWaitForever);
  ticks = info_handler->ticks_r;
  osMutexRelease(*(info_handler->mutexHandle));
  return ticks;
}

void noahInfo_set_target_speed_l(NoahInfoHandler *info_handler, float speed) {
  osMutexWait(*(info_handler->mutexHandle), osWaitForever);
  info_handler->target_speed_l = speed;
  osMutexRelease(*(info_handler->mutexHandle));
}

float noahInfo_get_target_speed_l(NoahInfoHandler *info_handler) {
  float speed;
  osMutexWait(*(info_handler->mutexHandle), osWaitForever);
  speed = info_handler->target_speed_l;
  osMutexRelease(*(info_handler->mutexHandle));
  return speed;
}

void noahInfo_set_target_speed_r(NoahInfoHandler *info_handler, float speed) {
  osMutexWait(*(info_handler->mutexHandle), osWaitForever);
  info_handler->target_speed_r = speed;
  osMutexRelease(*(info_handler->mutexHandle));
}

float noahInfo_get_target_speed_r(NoahInfoHandler *info_handler) {
  float speed;
  osMutexWait(*(info_handler->mutexHandle), osWaitForever);
  speed = info_handler->target_speed_r;
  osMutexRelease(*(info_handler->mutexHandle));
  return speed;
}

void noahInfo_set_imu_acceleration(NoahInfoHandler *info_handler, Vector3 acceleration) {
  osMutexWait(*(info_handler->mutexHandle), osWaitForever);
  imu_acceleration.x = acceleration.x;
  imu_acceleration.y = acceleration.y;
  imu_acceleration.z = acceleration.z;
  osMutexRelease(*(info_handler->mutexHandle));
}

Vector3 noahInfo_get_imu_acceleration(NoahInfoHandler *info_handler) {
  return acceleration;
}
