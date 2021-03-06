/******************************************************************************
arduino_mpu9250_clk.c - MPU-9250 Digital Motion Processor Arduino Library 
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
#include <External/SparkFun_MPU9250_DMP/src/util/stm32_mpu9250_clk.h>

#include "main.h"

int arduino_get_clock_ms(unsigned long *count)
{
	*count = HAL_GetTick();
	return 0;
}

int arduino_delay_ms(unsigned long num_ms)
{
	HAL_Delay(num_ms);
	return 0;
}
