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
#include <cstdlib>
#include <memory>

// CubeMx libraries
#include "stm32f407xx.h"

// Noah libraries
#include <Config.h>
#include <External/SparkFun_MPU_9250_DMP/src/SparkFunMPU9250-DMP.h>
#include <Tasks/IMU/IMU.hpp>

float acc_x, acc_y, acc_z;
float gyro_x, gyro_y, gyro_z;
float mag_x, mag_y, mag_z;
int16_t temp;

namespace noah {
namespace tasks {

IMU::IMU(NoahInfoHandler *noah_info_handler) :
    noah_info_handler { noah_info_handler } {
}

int IMU::run() {
  // Initialize IMU.
  auto result = imu.begin();
  if(result != INV_SUCCESS){
    return EXIT_FAILURE;
  }

  // Configure sensor.
  // Turn on all sensors.
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS); // Enable all sensors
  // Set gyro dps.
  imu.setGyroFSR(IMU_GYRO_DPS);
  // Set accel to +/-2g
  imu.setAccelFSR(IMU_ACCEL_G);
  // Set the digital low-pass filter
  // of the accelerometer and gyroscope.
  // (values are in Hz).
  imu.setLPF(IMU_LPF);
  // The sample rate of the accel/gyro.
  imu.setSampleRate(IMU_SAMPLE_RATE_HZ);
  // Set compass sample rate.
  imu.setCompassSampleRate(IMU_COMPASS_SAMPLE_RATE_HZ);

  while(1){
    // Vector3 accelerometer, gyro, magnetometer;
    imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS | UPDATE_TEMP);

    //Update acceleration values (values are in g's).
    acc_x = imu.calcAccel(imu.ax);
    acc_y = imu.calcAccel(imu.ay);
    acc_z = imu.calcAccel(imu.az);

    //Update gyroscope values (values are in dps).
    gyro_x = imu.calcGyro(imu.gx);
    gyro_y = imu.calcGyro(imu.gy);
    gyro_z = imu.calcGyro(imu.gz);
//    // Update magnetometer (values are in uT).
//    mag_x = imu.calcMag(imu.mx);
//    mag_y = imu.calcMag(imu.my);
//    mag_z = imu.calcMag(imu.mz);

    // Read temperateure.
//    temp = imu.temperature;

    osDelay(50);
  }

}

}
}
