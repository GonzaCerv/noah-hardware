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
#include "math.h"
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
float current_speed_l = 0.0, current_error_l, targetSpeed_l, integrationSum_l, derivative_l, previousError_l,
      speed_calc_l;
float current_speed_r = 0.0, current_error_r, targetSpeed_r, integrationSum_r, derivative_r, previousError_r,
      speed_calc_r;
float motorKP = 0.0f, motorKI = 0.0f, motorKD = 0.0f;

int16_t current_ticks_l = 0.0, duty_l, current_diff_l = 0;
int16_t current_ticks_r = 0.0, duty_r, current_diff_r = 0;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId pid_lHandle;
osThreadId pid_rHandle;
osThreadId usartHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void tsk_pid_l(void const * argument);
void tsk_pid_r(void const * argument);
void tsl_usart(void const * argument);

extern void MX_USB_DEVICE_Init(void);
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

  /* definition and creation of pid_l */
  osThreadDef(pid_l, tsk_pid_l, osPriorityNormal, 0, 300);
  pid_lHandle = osThreadCreate(osThread(pid_l), NULL);

  /* definition and creation of pid_r */
  osThreadDef(pid_r, tsk_pid_r, osPriorityNormal, 0, 300);
  pid_rHandle = osThreadCreate(osThread(pid_r), NULL);

  /* definition and creation of usart */
  osThreadDef(usart, tsl_usart, osPriorityIdle, 0, 300);
  usartHandle = osThreadCreate(osThread(usart), NULL);

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
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    HAL_GPIO_WritePin(SMPS_OFF_PORT, SMPS_OFF_PIN, GPIO_PIN_SET);
    // Wait until the button is released.
    while(HAL_GPIO_ReadPin(BUTTON_GPIO_Port,BUTTON_Pin) == GPIO_PIN_RESET);
    motorKP = PID_DEFAULT_KP;
    motorKI = PID_DEFAULT_KI;
    motorKD = PID_DEFAULT_KD;
    // Initialize the motor driver.
    motor_init();

    // Initialize the encoders.
    encoder_init();
    for (;;) {
        osDelay(20);
        if (usartComms_is_turn_off_requested()) {
            osDelay(OFF_WAIT_TIME_MS);
            HAL_GPIO_WritePin(SMPS_OFF_PORT, SMPS_OFF_PIN, GPIO_PIN_RESET);
        }
        if(HAL_GPIO_ReadPin(BUTTON_GPIO_Port,BUTTON_Pin) == GPIO_PIN_SET){
            osDelay(200);
            if(HAL_GPIO_ReadPin(BUTTON_GPIO_Port,BUTTON_Pin) == GPIO_PIN_SET){
              HAL_GPIO_WritePin(AUX_LED1_GPIO_Port,AUX_LED1_Pin,GPIO_PIN_SET);
              usartComms_publish_off();
              osDelay(OFF_WAIT_TIME_MS);
              HAL_GPIO_WritePin(SMPS_OFF_PORT, SMPS_OFF_PIN, GPIO_PIN_RESET);
            }
        }
    }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_tsk_pid_l */
/**
* @brief Function implementing the pid_l thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_tsk_pid_l */
void tsk_pid_l(void const * argument)
{
  /* USER CODE BEGIN tsk_pid_l */
    int16_t ticks_l_now = 0;
    int16_t ticks_l_prev = 0;
  /* Infinite loop */
  for(;;)
  {
    //Wait for the next update of the encoder values.
    osSignalWait(0x00000001,osWaitForever);

    //Retrieve the target speed.
    targetSpeed_l = usartComms_get_target_speed_l();

    //Calculate the current speed of the motor.
    ticks_l_now = (int16_t) current_ticks_l;
    current_diff_l =  encoder_calculate_diff(ticks_l_prev,ticks_l_now); 
    current_speed_l = ((((float)current_diff_l / (ENCODER_UPDATE_INTERVAL_MS / 1000.0f)) / STEPS_PER_TURN) * 2 * (float)M_PI * WHEEL_RADIUS);

    //Calculate the PID.
    current_error_l = targetSpeed_l - current_speed_l;
    integrationSum_l += (current_error_l * (ENCODER_UPDATE_INTERVAL_MS / 1000.0f));
    if(integrationSum_l > MAX_INTEGRATION_SUM) {
      integrationSum_l = MAX_INTEGRATION_SUM;
    }
    else if(fabs(integrationSum_l) > MAX_INTEGRATION_SUM){
      integrationSum_l = -MAX_INTEGRATION_SUM;
    }
    derivative_l = (current_error_l - previousError_l) / (ENCODER_UPDATE_INTERVAL_MS / 1000.0f);
    duty_l = (int16_t)((motorKP * current_error_l) + (motorKI * integrationSum_l) + (motorKD * derivative_l));

    //Limit the max and min possible values for PID.
    if(duty_l > PWM_MAX_VAL){
      duty_l = PWM_MAX_VAL;
    }
    else if(abs(duty_l) < PWM_MIN_VAL){
      duty_l = 0;
    }
    else if(abs(duty_l) > PWM_MAX_VAL){
      duty_l = -PWM_MAX_VAL;
    }

    //Save the values for the next iteration.
    previousError_l = current_error_l;
    ticks_l_prev = ticks_l_now;

    //Apply the PID.
    motor_set_duty(MOTOR_L, (int32_t)duty_l);
  }
  /* USER CODE END tsk_pid_l */
}

/* USER CODE BEGIN Header_tsk_pid_r */
/**
* @brief Function implementing the pid_r thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_tsk_pid_r */
void tsk_pid_r(void const * argument)
{
  /* USER CODE BEGIN tsk_pid_r */
    int16_t ticks_r_now = 0;
    int16_t ticks_r_prev = 0;
  /* Infinite loop */
  for(;;)
  {
    //Wait for the next update of the encoder values.
    osSignalWait(0x00000001,osWaitForever);

    //Retrieve the target speed.
    targetSpeed_r = usartComms_get_target_speed_r();

    //Calculate the current speed of the motor.
    ticks_r_now = (int16_t) current_ticks_r;
    current_diff_r =  encoder_calculate_diff(ticks_r_prev,ticks_r_now); 
    current_speed_r = ((((float)current_diff_r / (ENCODER_UPDATE_INTERVAL_MS / 1000.0f)) / STEPS_PER_TURN) * 2 * (float)M_PI * WHEEL_RADIUS);

    //Calculate the PID.
    current_error_r = targetSpeed_r - current_speed_r;
    integrationSum_r += (current_error_r * (ENCODER_UPDATE_INTERVAL_MS / 1000.0f));
    if(integrationSum_r > MAX_INTEGRATION_SUM) {
      integrationSum_r = MAX_INTEGRATION_SUM;
    }
    else if(fabs(integrationSum_r) > MAX_INTEGRATION_SUM){
      integrationSum_r = -MAX_INTEGRATION_SUM;
    }
    derivative_r = (current_error_r - previousError_r) / (ENCODER_UPDATE_INTERVAL_MS / 1000.0f);
    duty_r = (int16_t)((motorKP * current_error_r) + (motorKI * integrationSum_r) + (motorKD * derivative_r));

    //Limit the max and min possible values for PID.
    if(duty_r > PWM_MAX_VAL){
      duty_r = PWM_MAX_VAL;
    }
    else if(abs(duty_r) < PWM_MIN_VAL){
      duty_r = 0;
    }
    else if(abs(duty_r) > PWM_MAX_VAL){
      duty_r = -PWM_MAX_VAL;
    }

    //Save the values for the next iteration.
    previousError_r = current_error_r;
    ticks_r_prev = ticks_r_now;

    //Apply the PID.
    motor_set_duty(MOTOR_R, (int32_t)duty_r);
  }
  /* USER CODE END tsk_pid_r */
}

/* USER CODE BEGIN Header_tsl_usart */
/**
* @brief Function implementing the usart thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_tsl_usart */
void tsl_usart(void const * argument)
{
  /* USER CODE BEGIN tsl_usart */
  usartComms_init();
  /* Infinite loop */
  for(;;)
  {
    usartComms_update();
    osDelay(UART_COMMS_UPDATE_RATE_MS);
  }
  /* USER CODE END tsl_usart */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
