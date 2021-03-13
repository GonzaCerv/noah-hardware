/******************************************************************************
 arduino_mpu9250_i2c.cpp - MPU-9250 Digital Motion Processor Arduino Library
 Jim Lindblom @ SparkFun Electronics
 original creation date: November 23, 2016
 https://github.com/sparkfun/SparkFun_MPU9250_DMP_Arduino_Library

 This library implements motion processing functions of Invensense's MPU-9250.
 It is based on their Emedded MotionDriver 6.12 library.
 https://www.invensense.com/developers/software-downloads/

 Development environment specifics:
 Arduino IDE 1.6.12
 SparkFun 9DoF Razor IMU M0

 Supported Platforms:
 - ATSAMD21 (Arduino Zero, SparkFun SAMD21 Breakouts)
 ******************************************************************************/
#include <External/SparkFun_MPU_9250_DMP/src/util/arduino_mpu9250_i2c.h>

#include <cstdint>
#include <Config.h>
#include "main.h"

int stm32_i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data) {
  HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&IMU_I2C_PORT, (uint16_t) slave_addr<<1, (uint8_t*) data, (uint16_t) length, 20);
  if(status != HAL_OK){
    return 1;
  }
  return 0;
}

int stm32_i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data) {
  HAL_StatusTypeDef status = HAL_I2C_Master_Receive(&IMU_I2C_PORT, (uint16_t) slave_addr<<1, (uint8_t*) data, (uint16_t) length, 20);
  if(status != HAL_OK){
    return 1;
  }
  return 0;
}
