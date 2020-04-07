/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define GREEN_LED_Pin GPIO_PIN_13
#define GREEN_LED_GPIO_Port GPIOC
#define RED_LED_Pin GPIO_PIN_14
#define RED_LED_GPIO_Port GPIOC
#define BAT_Pin GPIO_PIN_0
#define BAT_GPIO_Port GPIOA
#define PWMA_Pin GPIO_PIN_1
#define PWMA_GPIO_Port GPIOA
#define PWMB_Pin GPIO_PIN_2
#define PWMB_GPIO_Port GPIOA
#define MOTB1_Pin GPIO_PIN_4
#define MOTB1_GPIO_Port GPIOA
#define MOTB2_Pin GPIO_PIN_5
#define MOTB2_GPIO_Port GPIOA
#define MOT_IRA_Pin GPIO_PIN_6
#define MOT_IRA_GPIO_Port GPIOA
#define MOT_IRB_Pin GPIO_PIN_7
#define MOT_IRB_GPIO_Port GPIOA
#define BQ24070_STAT2_Pin GPIO_PIN_0
#define BQ24070_STAT2_GPIO_Port GPIOB
#define BQ24070_STAT1_Pin GPIO_PIN_1
#define BQ24070_STAT1_GPIO_Port GPIOB
#define BQ24070_MODE_Pin GPIO_PIN_2
#define BQ24070_MODE_GPIO_Port GPIOB
#define BUTTON_Pin GPIO_PIN_14
#define BUTTON_GPIO_Port GPIOB
#define FLOOR_IRF_Pin GPIO_PIN_15
#define FLOOR_IRF_GPIO_Port GPIOB
#define MOTA1_Pin GPIO_PIN_8
#define MOTA1_GPIO_Port GPIOA
#define FLOOR_IRB_Pin GPIO_PIN_15
#define FLOOR_IRB_GPIO_Port GPIOA
#define MOTA2_Pin GPIO_PIN_4
#define MOTA2_GPIO_Port GPIOB
#define EN_5V_Pin GPIO_PIN_5
#define EN_5V_GPIO_Port GPIOB
#define SERVO_PWM_Pin GPIO_PIN_7
#define SERVO_PWM_GPIO_Port GPIOB
#define EN_6V_Pin GPIO_PIN_8
#define EN_6V_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
