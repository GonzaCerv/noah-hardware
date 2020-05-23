/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MOT_L1_Pin GPIO_PIN_2
#define MOT_L1_GPIO_Port GPIOE
#define MOT_L2_Pin GPIO_PIN_3
#define MOT_L2_GPIO_Port GPIOE
#define MOT_STAND_BY_Pin GPIO_PIN_4
#define MOT_STAND_BY_GPIO_Port GPIOE
#define AUX_LED1_Pin GPIO_PIN_14
#define AUX_LED1_GPIO_Port GPIOC
#define AUX_LED2_Pin GPIO_PIN_15
#define AUX_LED2_GPIO_Port GPIOC
#define BUTTON_Pin GPIO_PIN_0
#define BUTTON_GPIO_Port GPIOC
#define ESP_REGULAR_FLASH_Pin GPIO_PIN_1
#define ESP_REGULAR_FLASH_GPIO_Port GPIOC
#define STATUS_OUT0_Pin GPIO_PIN_3
#define STATUS_OUT0_GPIO_Port GPIOC
#define SENS_A_L_Pin GPIO_PIN_0
#define SENS_A_L_GPIO_Port GPIOA
#define SENS_B_L_Pin GPIO_PIN_1
#define SENS_B_L_GPIO_Port GPIOA
#define GPIO0_Pin GPIO_PIN_2
#define GPIO0_GPIO_Port GPIOA
#define GPIO1_Pin GPIO_PIN_3
#define GPIO1_GPIO_Port GPIOA
#define EN_POWER_3V3_Pin GPIO_PIN_4
#define EN_POWER_3V3_GPIO_Port GPIOA
#define SENS_A_R_Pin GPIO_PIN_6
#define SENS_A_R_GPIO_Port GPIOA
#define SENS_B_R_Pin GPIO_PIN_7
#define SENS_B_R_GPIO_Port GPIOA
#define GPIO2_Pin GPIO_PIN_4
#define GPIO2_GPIO_Port GPIOC
#define GPIO3_Pin GPIO_PIN_5
#define GPIO3_GPIO_Port GPIOC
#define GPIO4_Pin GPIO_PIN_0
#define GPIO4_GPIO_Port GPIOB
#define GPIO5_Pin GPIO_PIN_1
#define GPIO5_GPIO_Port GPIOB
#define CHARGER_CONNECTED_Pin GPIO_PIN_8
#define CHARGER_CONNECTED_GPIO_Port GPIOE
#define ENABLE_PRESSED_Pin GPIO_PIN_9
#define ENABLE_PRESSED_GPIO_Port GPIOE
#define ON_BUTTON_Pin GPIO_PIN_10
#define ON_BUTTON_GPIO_Port GPIOE
#define IMU_CSB_Pin GPIO_PIN_11
#define IMU_CSB_GPIO_Port GPIOE
#define IMU_NCS_Pin GPIO_PIN_12
#define IMU_NCS_GPIO_Port GPIOE
#define IMU_SDO_Pin GPIO_PIN_13
#define IMU_SDO_GPIO_Port GPIOE
#define ESP07_EN_Pin GPIO_PIN_14
#define ESP07_EN_GPIO_Port GPIOE
#define ESP07_RST_Pin GPIO_PIN_15
#define ESP07_RST_GPIO_Port GPIOE
#define GPIO6_Pin GPIO_PIN_12
#define GPIO6_GPIO_Port GPIOB
#define GPIO7_Pin GPIO_PIN_13
#define GPIO7_GPIO_Port GPIOB
#define GPIO8_Pin GPIO_PIN_14
#define GPIO8_GPIO_Port GPIOB
#define GPIO9_Pin GPIO_PIN_15
#define GPIO9_GPIO_Port GPIOB
#define EN_SMPS_Pin GPIO_PIN_8
#define EN_SMPS_GPIO_Port GPIOD
#define OUT1_Pin GPIO_PIN_9
#define OUT1_GPIO_Port GPIOD
#define OUT2_Pin GPIO_PIN_10
#define OUT2_GPIO_Port GPIOD
#define OUT3_Pin GPIO_PIN_11
#define OUT3_GPIO_Port GPIOD
#define OUT4_Pin GPIO_PIN_12
#define OUT4_GPIO_Port GPIOD
#define MOT_PWM_L_Pin GPIO_PIN_13
#define MOT_PWM_L_GPIO_Port GPIOD
#define MOT_PWM_R_Pin GPIO_PIN_14
#define MOT_PWM_R_GPIO_Port GPIOD
#define SERVO0_Pin GPIO_PIN_6
#define SERVO0_GPIO_Port GPIOC
#define SERVO1_Pin GPIO_PIN_7
#define SERVO1_GPIO_Port GPIOC
#define SERVO2_Pin GPIO_PIN_8
#define SERVO2_GPIO_Port GPIOC
#define SERVO3_Pin GPIO_PIN_9
#define SERVO3_GPIO_Port GPIOC
#define STATUS_OUT1_Pin GPIO_PIN_12
#define STATUS_OUT1_GPIO_Port GPIOC
#define STATUS_OUT2_Pin GPIO_PIN_2
#define STATUS_OUT2_GPIO_Port GPIOD
#define STATUS_OUT3_Pin GPIO_PIN_3
#define STATUS_OUT3_GPIO_Port GPIOD
#define STATUS_OUT4_Pin GPIO_PIN_4
#define STATUS_OUT4_GPIO_Port GPIOD
#define GPS_RX_Pin GPIO_PIN_5
#define GPS_RX_GPIO_Port GPIOD
#define GPS_TX_Pin GPIO_PIN_6
#define GPS_TX_GPIO_Port GPIOD
#define OUT0_Pin GPIO_PIN_7
#define OUT0_GPIO_Port GPIOD
#define MOT_R2_Pin GPIO_PIN_0
#define MOT_R2_GPIO_Port GPIOE
#define MOT_R1_Pin GPIO_PIN_1
#define MOT_R1_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
