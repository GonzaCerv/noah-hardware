/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     

// Standard libraries
#include "stdint.h"

// CubeMx libraries
#include "tim.h"
#include "usart.h"

// Noah libraries
#include "Config.h"
#include "Encoder.h"
#include "Motor.h"
#include "NoahUtils.h"
#include "UsartCommunication.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
float current_speed_l = 0.0, current_error_l, targetSpeed_l, integrationSum_l, derivative_l, previousError_l, speed_calc_l;
float current_speed_r = 0.0, current_error_r, targetSpeed_r, integrationSum_r, derivative_r, previousError_r, speed_calc_r;
float motorKP = 0.0f ,motorKI = 0.0f, motorKD = 0.0f;

int32_t last_count_l = 0.0, current_count_l = 0.0, duty_l, current_diff_l = 0 ;
int32_t last_count_r = 0.0, current_count_r = 0.0, duty_r, current_diff_r = 0 ;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId EncoderHandle;
osThreadId PIDHandle;
osThreadId UsartHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void tsk_encoder(void const * argument);
void tsk_pid(void const * argument);
void tsk_usart(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Encoder */
  osThreadDef(Encoder, tsk_encoder, osPriorityNormal, 0, 300);
  EncoderHandle = osThreadCreate(osThread(Encoder), NULL);

  /* definition and creation of PID */
  osThreadDef(PID, tsk_pid, osPriorityNormal, 0, 300);
  PIDHandle = osThreadCreate(osThread(PID), NULL);

  /* definition and creation of Usart */
  osThreadDef(Usart, tsk_usart, osPriorityNormal, 0, 300);
  UsartHandle = osThreadCreate(osThread(Usart), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_tsk_encoder */
/**
* @brief Function implementing the Encoder thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_tsk_encoder */
void tsk_encoder(void const * argument)
{
  /* USER CODE BEGIN tsk_encoder */
  /* Infinite loop */
    int32_t *ptr_l;
    int32_t *ptr_r;
    int32_t diff_l[ENCODER_BUFFER_SIZE];
    int32_t diff_r[ENCODER_BUFFER_SIZE];
    uint16_t  aux;
    for (aux = 0; aux < ENCODER_BUFFER_SIZE; ++aux) {
        diff_l[aux] = 0;
        diff_r[aux] = 0;
    }
    ptr_l = diff_l;
    ptr_r = diff_r;
    HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
    HAL_TIM_Base_Start_IT (&htim7);
    /* Infinite loop */
    for (;;) {
        osDelay((uint32_t)ENCODER_UPDATE_INTERVAL_MS);

        // SpeedCalculation encoder L
        if (isCountingUpL()) {
          if (current_count_l >= last_count_l) {
              current_diff_l = current_count_l - last_count_l;
          } else {
              current_diff_l = (ENCODER_TIMER_MAX_VAL - last_count_l) + current_count_l;
          }
        } else {
          if (current_count_l > last_count_l) {
              current_diff_l = (ENCODER_TIMER_MAX_VAL - current_count_l) + last_count_l;
          } else {
              current_diff_l = (last_count_l - current_count_l);
          }
          current_diff_l = -current_diff_l;
        }
        *ptr_l = current_diff_l;
        ++ptr_l;
        if (ptr_l == (diff_l + ENCODER_BUFFER_SIZE)) {
          ptr_l = diff_l;
          sortArray(diff_l, ENCODER_BUFFER_SIZE);
          speed_calc_l = MeanCalc(diff_l, ENCODER_BUFFER_SIZE, ENCODER_MEAN_DISCARD_COUNT);
          speed_calc_l = (((speed_calc_l / (ENCODER_UPDATE_INTERVAL_MS / 1000.0f)) / STEPS_PER_TURN) * 2 * (float)M_PI * WHEEL_RADIUS);
          current_speed_l = applyResolutiom(speed_calc_l, SPEED_RESOLUTION);
        }

        // SpeedCalculation encoder R
        if (isCountingUpR()) {
          if (current_count_r >= last_count_r) {
              current_diff_r = current_count_r - last_count_r;
          } else {
              current_diff_r = (ENCODER_TIMER_MAX_VAL - last_count_r) + current_count_r;
          }
        } else {
          if (current_count_r > last_count_r) {
              current_diff_r = (ENCODER_TIMER_MAX_VAL - current_count_r) + last_count_r;
          } else {
              current_diff_r = (last_count_r - current_count_r);
          }
          current_diff_r = -current_diff_r;
        }
        *ptr_r = current_diff_r;
        ++ptr_r;
        if (ptr_r == (diff_r + ENCODER_BUFFER_SIZE)) {
          ptr_r = diff_r;
          sortArray(diff_r, ENCODER_BUFFER_SIZE);
          speed_calc_r = MeanCalc(diff_r, ENCODER_BUFFER_SIZE, ENCODER_MEAN_DISCARD_COUNT);
          speed_calc_r = (((speed_calc_r / (ENCODER_UPDATE_INTERVAL_MS / 1000.0f)) / STEPS_PER_TURN) * 2 * (float)M_PI * WHEEL_RADIUS);
          current_speed_r = applyResolutiom(speed_calc_r, 0.02);
        }
    }
  /* USER CODE END tsk_encoder */
}

/* USER CODE BEGIN Header_tsk_pid */
/**
* @brief Function implementing the PID thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_tsk_pid */
void tsk_pid(void const * argument)
{
  /* USER CODE BEGIN tsk_pid */

	// Initialize PID system, float32_t format.
  targetSpeed_l = 0.0f;
  targetSpeed_r = 0.0f;
  motorKP = PID_PARAM_KP;
  motorKI = PID_PARAM_KI;
  motorKD = PID_PARAM_KD;

  // Initialize the motor driver.
  motor_init();

    /* Infinite loop */
    for (;;) {
        osDelay((uint32_t)PID_UPDATE_RATE_MS);

        // Implementación PID for motor L.
        current_error_l = targetSpeed_l - current_speed_l;
        // We divide by 1000 to convert miliseconds in seconds.
        integrationSum_l += (current_error_l * (PID_UPDATE_RATE_MS / 1000.0f));
        if(integrationSum_l > MAX_INTEGRATION_SUM) {
          integrationSum_l = MAX_INTEGRATION_SUM;
        }
        else if(fabs(integrationSum_l) > MAX_INTEGRATION_SUM){
          integrationSum_l = -MAX_INTEGRATION_SUM;
        }
        derivative_l = (current_error_l - previousError_l) / (PID_UPDATE_RATE_MS / 1000.0f);
        duty_l = (int32_t)((motorKP * current_error_l) + (motorKI * integrationSum_l) + (motorKD * derivative_l));
        previousError_l = current_error_l;
        if (duty_l > MAX_DUTY_PWM){
          duty_l = MAX_DUTY_PWM;
        }
        else if(abs(duty_l) < MIN_DUTY_PWM) {
          duty_l = 0;
        }
        else if(abs(duty_l) > MAX_DUTY_PWM){
          duty_l = -MAX_DUTY_PWM;
        }
        motor_set_duty(MOTOR_L, duty_l);

        // Implementación PID for motor R.
        current_error_r = targetSpeed_r - current_speed_r;
        // We divide by 1000 to convert miliseconds in seconds.
        integrationSum_r += (current_error_r * (PID_UPDATE_RATE_MS / 1000.0f));
        if(integrationSum_r > MAX_INTEGRATION_SUM) {
          integrationSum_r = MAX_INTEGRATION_SUM;
        }
        else if(fabs(integrationSum_r) > MAX_INTEGRATION_SUM){
          integrationSum_r = -MAX_INTEGRATION_SUM;
        }
        derivative_r = (current_error_r - previousError_r) / (PID_UPDATE_RATE_MS / 1000.0f);
        duty_r = (int32_t)((motorKP * current_error_r) + (motorKI * integrationSum_r) + (motorKD * derivative_r));
        previousError_r = current_error_r;
        if (duty_r > MAX_DUTY_PWM){
          duty_r = MAX_DUTY_PWM;
        }
        else if(abs(duty_r) < MIN_DUTY_PWM) {
          duty_r = 0;
        }
        else if(abs(duty_r) > MAX_DUTY_PWM){
          duty_r = -MAX_DUTY_PWM;
        }
        motor_set_duty(MOTOR_R, duty_r);
    }
  /* USER CODE END tsk_pid */
}

/* USER CODE BEGIN Header_tsk_usart */
/**
* @brief Function implementing the Usart thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_tsk_usart */
void tsk_usart(void const * argument)
{
  /* USER CODE BEGIN tsk_usart */
  usartComms_init();
  /* Infinite loop */
  for(;;)
  {
    usartComms_update();
    osDelay(UART_COMMS_UPDATE_RATE_MS);
  }
  /* USER CODE END tsk_usart */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
